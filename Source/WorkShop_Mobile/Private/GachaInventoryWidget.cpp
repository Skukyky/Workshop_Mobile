#include "GachaInventoryWidget.h"
#include "BTNCustomWidget.h"
#include "GachaCharacterShowcase.h"
#include "GachaInventoryItemWidget.h"
#include "Components/ScrollBox.h"
#include "PlayerActor.h"
#include "RoomWorking.h"
#include "Worker.h"
#include "WorkRoomSettingWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"

void UGachaInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Récupérer la référence au PlayerActor
    PlayerActor = Cast<APlayerActor>(GetWorld()->GetFirstPlayerController()->GetPawn());
    
    if (PlayerActor != nullptr)
    {
        // Peupler l'inventaire directement depuis le PlayerActor
        PopulateInventoryFromPlayer();
    }
    else if (InventoryScrollBox)
    {
        InventoryScrollBox->ClearChildren();
    }

    if (BTN_LostFocus)
    {
        BTN_LostFocus->OnCustomButtonClicked.AddDynamic(this, &UGachaInventoryWidget::OnLostFocusClicked);
    }

    if (BTN_Assign)
    {
        BTN_Assign->OnCustomButtonClicked.AddDynamic(this, &UGachaInventoryWidget::OnAssignClicked);
    }
}

void UGachaInventoryWidget::PopulateInventoryFromPlayer()
{
    if (!InventoryScrollBox || !CharacterDataTable || !PlayerActor)
    {
        return;
    }

    InventoryScrollBox->ClearChildren();

    // Récupérer le tableau directement depuis le PlayerActor
    TArray<FCharacterProgress>& CharactersInventory = PlayerActor->GetWorkersInventory();

    if (CharactersInventory.Num() == 0)
    {
        return;
    }

    // Copie locale pour tri
    TArray<FCharacterProgress> SortedInventory = CharactersInventory;
    
    // Tri par rareté descendante, étoiles descendantes, puis nom ascendante
    SortedInventory.Sort([this](const FCharacterProgress& A, const FCharacterProgress& B)
    {
        FCharacterStructure* DataA = CharacterDataTable->FindRow<FCharacterStructure>(A.CharacterID, TEXT(""));
        FCharacterStructure* DataB = CharacterDataTable->FindRow<FCharacterStructure>(B.CharacterID, TEXT(""));
        
        int32 RarityA = DataA ? static_cast<int32>(DataA->Rarity) : 0;
        int32 RarityB = DataB ? static_cast<int32>(DataB->Rarity) : 0;

        if (RarityA != RarityB)
        {
            return RarityA > RarityB;
        }

        if (A.StarCount != B.StarCount)
        {
            return A.StarCount > B.StarCount;
        }

        return A.CharacterID.ToString() < B.CharacterID.ToString();
    });

    for (const FCharacterProgress& Progress : SortedInventory)
    {
        bool ShouldDisplay = true;

        // Vérifier si le worker est déjà assigné
        if (AssignButtonReturn)
        {
            if (AssignButtonReturn->WorkRoomSettingWidget->RoomWorking)
            {
                IsAllReadyUse = false;
                for (AWorker* Worker : PlayerActor->Workers)
                {
                    if (Worker == Progress.WorkerSpawnRef)
                    {
                        IsAllReadyUse = true;
                        ShouldDisplay = false;
                        break;
                    }
                }
            }
        }

        if (!ShouldDisplay)
            continue;

        const FName& CharacterRowName = Progress.CharacterID;
        FCharacterStructure* CharacterData = CharacterDataTable->FindRow<FCharacterStructure>(CharacterRowName, TEXT("GachaInventoryWidget::PopulateInventory"));
        
        if (!CharacterData || !ItemWidgetClass)
        {
            continue;
        }

        UGachaInventoryItemWidget* EntryWidget = CreateWidget<UGachaInventoryItemWidget>(this, ItemWidgetClass);
        if (!EntryWidget)
        {
            continue;
        }

        EntryWidget->InitializeWithData(*CharacterData, Progress, Progress.CharacterID);
        EntryWidget->OnItemSelected.AddDynamic(this, &UGachaInventoryWidget::OnItemSelected);
        InventoryScrollBox->AddChild(EntryWidget);
    }
}

void UGachaInventoryWidget::OnItemSelected(UGachaInventoryItemWidget* ClickedItem)
{
    SelectedItemWidget = ClickedItem;

    if (CharacterImage)
    {
        CharacterImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }

    if (StatImage)
    {
        StatImage->SetVisibility(ESlateVisibility::Visible);
        
        FName SelectedCharacterID = SelectedItemWidget ? SelectedItemWidget->GetCharacterID() : NAME_None;
        if (SelectedCharacterID != NAME_None && CharacterDataTable)
        {
            const FCharacterStructure* CharacterRow = CharacterDataTable->FindRow<FCharacterStructure>(SelectedCharacterID, TEXT("OnItemSelected"));
            if (CharacterRow && CharacterRow->CV_Character)
            {
                FSlateBrush NewBrush;
                NewBrush.SetResourceObject(CharacterRow->CV_Character);
                NewBrush.ImageSize = FVector2D(CharacterRow->CV_Character->GetSizeX(), CharacterRow->CV_Character->GetSizeY());
                StatImage->SetBrush(NewBrush);
            }
            else
            {
                StatImage->SetBrush(FSlateNoResource());
            }
        }
    }

    if (PB_Tiktok)
    {
        FName SelectedCharacterID = SelectedItemWidget ? SelectedItemWidget->GetCharacterID() : NAME_None;
        const FCharacterStructure* CharacterRow = CharacterDataTable->FindRow<FCharacterStructure>(SelectedCharacterID, TEXT("OnItemSelected"));
        PB_Tiktok->SetVisibility(ESlateVisibility::Visible);
        float StarPercent = CharacterRow->StatTikTok / 5;
        PB_Tiktok->SetPercent(StarPercent);
    }

    if (PB_Youtube)
    {
        FName SelectedCharacterID = SelectedItemWidget ? SelectedItemWidget->GetCharacterID() : NAME_None;
        const FCharacterStructure* CharacterRow = CharacterDataTable->FindRow<FCharacterStructure>(SelectedCharacterID, TEXT("OnItemSelected"));
        PB_Youtube->SetVisibility(ESlateVisibility::Visible);
        float StarPercent = CharacterRow->StatYoutube / 5;
        PB_Youtube->SetPercent(StarPercent);
    }

    if (BTN_Assign)
    {
        BTN_Assign->SetVisibility(ESlateVisibility::Visible);
    }

    // Détruire l'ancien showcase
    if (CurrentCharacterShowcase)
    {
        CurrentCharacterShowcase->Destroy();
        CurrentCharacterShowcase = nullptr;
    }

    // Spawn nouveau showcase
    if (CharacterShowcaseActorClass && GetWorld())
    {
        FVector SpawnLocation = FVector(0.f, 0.f, 100000);
        FRotator SpawnRotation = FRotator::ZeroRotator;
        FTransform SpawnTransform(SpawnRotation, SpawnLocation);

        CurrentCharacterShowcase = GetWorld()->SpawnActor<AGachaCharacterShowcase>(CharacterShowcaseActorClass, SpawnTransform);
        if (CurrentCharacterShowcase && SelectedItemWidget)
        {
            FName CharacterID = SelectedItemWidget->GetCharacterID();
            CurrentCharacterShowcase->CharacterDataTable = CharacterDataTable;
            CurrentCharacterShowcase->SetCharacterByRowName(CharacterID);
        }
    }
}

void UGachaInventoryWidget::OnLostFocusClicked()
{
    if (CharacterImage)
    {
        CharacterImage->SetVisibility(ESlateVisibility::Collapsed);
    }

    if (StatImage)
    {
        StatImage->SetVisibility(ESlateVisibility::Collapsed);
    }

    if (BTN_Assign)
    {
        BTN_Assign->SetVisibility(ESlateVisibility::Collapsed);
    }

    PB_Youtube->SetVisibility(ESlateVisibility::Collapsed);
    PB_Tiktok->SetVisibility(ESlateVisibility::Collapsed);

    if (CurrentCharacterShowcase)
    {
        CurrentCharacterShowcase->Destroy();
        CurrentCharacterShowcase = nullptr;
    }

    SelectedItemWidget = nullptr;
}

void UGachaInventoryWidget::OnAssignClicked()
{
    if (AssignButtonReturn && SelectedItemWidget && PlayerActor)
    {
        TArray<FCharacterProgress>& Inventory = PlayerActor->GetWorkersInventory();
        
        for (int i = 0; i < Inventory.Num(); i++)
        {
            if (Inventory[i].CharacterID == SelectedItemWidget->ProgressRef.CharacterID && 
                Inventory[i].StarCount == SelectedItemWidget->ProgressRef.StarCount &&
                Inventory[i].WorkerSpawnRef == SelectedItemWidget->ProgressRef.WorkerSpawnRef)
            {
                AssignButtonReturn->WorkRoomSettingWidget->RoomWorking->AddWorker(
                    AssignButtonReturn->Position,
                    Inventory[i].WorkerSpawnRef
                );
                break;
            }
        }
    }
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (PlayerController)
    {
        FInputModeGameOnly InputMode;
        PlayerController->SetInputMode(InputMode);
        PlayerController->bShowMouseCursor = true;
    }
    RemoveFromParent();
}