#include "GachaInventoryWidget.h"
#include "BTNCustomWidget.h"
#include "GachaCharacterShowcase.h"
#include "GachaInventoryItemWidget.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"
#include "GachaSaveGame.h"
#include "PlayerActor.h"
#include "RoomWorking.h"
#include "Worker.h"
#include "WorkRoomSettingWidget.h"
#include "Components/Image.h"

void UGachaInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();
/*
    UGachaSaveGame* LoadedGame = nullptr;
    if (UGameplayStatics::DoesSaveGameExist(TEXT("GachaSaveSlot"), 0))
    {
        LoadedGame = Cast<UGachaSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("GachaSaveSlot"), 0));
    }*/
    
    PlayerActor = Cast<APlayerActor>(GetWorld()->GetFirstPlayerController()->GetPawn());
    if (PlayerActor != nullptr)
    {
        PopulateInventory(PlayerActor->CharactersInventory);
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

void UGachaInventoryWidget::PopulateInventory(const TArray<FCharacterProgress>& CharactersInventory)
{
    if (!InventoryScrollBox || !CharacterDataTable)
    {
        return;
    }

    InventoryScrollBox->ClearChildren();

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
        if(AssignButtonReturn)
        {
            if(AssignButtonReturn->WorkRoomSettingWidget->RoomWorking)
            {
                IsAllReadyUse = false;
                for (FWorkerAssigned Worker : AssignButtonReturn->WorkRoomSettingWidget->RoomWorking->Workers)
                {
                    if (Worker.Worker == Progress.WorkerSpawnRef) IsAllReadyUse = true;
                }
                if (!IsAllReadyUse)
                {
                    const FName& CharacterRowName = Progress.CharacterID;
                    FCharacterStructure* CharacterData = CharacterDataTable->FindRow<FCharacterStructure>(CharacterRowName, TEXT("GachaInventoryWidget::PopulateInventory"));
                    if (!CharacterData)
                    {
                        continue;
                    }

                    if (!ItemWidgetClass)
                    {
                        continue;
                    }

                    UGachaInventoryItemWidget* EntryWidget = CreateWidget<UGachaInventoryItemWidget>(this, ItemWidgetClass);
                    if (!EntryWidget)
                    {
                        continue;
                    }

                    EntryWidget->InitializeWithData(*CharacterData, Progress, Progress.CharacterID);

                    // Bind event selection
                    EntryWidget->OnItemSelected.AddDynamic(this, &UGachaInventoryWidget::OnItemSelected);

                    InventoryScrollBox->AddChild(EntryWidget);
                }
            }
            
            
        }
        else
        {
            const FName& CharacterRowName = Progress.CharacterID;
            FCharacterStructure* CharacterData = CharacterDataTable->FindRow<FCharacterStructure>(CharacterRowName, TEXT("GachaInventoryWidget::PopulateInventory"));
            if (!CharacterData)
            {
                continue;
            }

            if (!ItemWidgetClass)
            {
                continue;
            }

            UGachaInventoryItemWidget* EntryWidget = CreateWidget<UGachaInventoryItemWidget>(this, ItemWidgetClass);
            if (!EntryWidget)
            {
                continue;
            }

            EntryWidget->InitializeWithData(*CharacterData, Progress, Progress.CharacterID);

            // Bind event selection
            EntryWidget->OnItemSelected.AddDynamic(this, &UGachaInventoryWidget::OnItemSelected);

            InventoryScrollBox->AddChild(EntryWidget);
        }
    }
}

void UGachaInventoryWidget::OnItemSelected(UGachaInventoryItemWidget* ClickedItem)
{
    SelectedItemWidget = ClickedItem;

    if (CharacterImage)
    {
        CharacterImage->SetIsEnabled(false);
        CharacterImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }

    if (StatImage)
    {
        StatImage->SetIsEnabled(false);
        StatImage->SetVisibility(ESlateVisibility::Visible);
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

    // Spawn nouveau showcase en utilisant la hauteur configurable
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
        for (int i = 0;PlayerActor->CharactersInventory.Num() -1 >= i; i++)
        {
            if (PlayerActor->CharactersInventory[i].CharacterID == SelectedItemWidget->ProgressRef.CharacterID && PlayerActor->CharactersInventory[i].StarCount == SelectedItemWidget->ProgressRef.StarCount)
            {
                AssignButtonReturn->WorkRoomSettingWidget->RoomWorking->AddWorker(AssignButtonReturn->Position,PlayerActor->CharactersInventory[i].WorkerSpawnRef);
            }
        }
        RemoveFromParent();
    }
}
