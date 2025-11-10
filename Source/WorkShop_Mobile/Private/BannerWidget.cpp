#include "BannerWidget.h"
#include "GachaPullWidget.h"
#include "Components/Image.h"
#include "PlayerActor.h"
#include "Kismet/GameplayStatics.h"

void UBannerWidget::SetParentGachaWidget(UGachaPullWidget* Parent)
{
    ParentGachaWidget = Parent;
}

void UBannerWidget::NativePreConstruct()
{
    Super::NativePreConstruct();
    if (ImageBackground && BackgroundTexture)
    {
        FSlateBrush NewBrush;
        NewBrush.SetResourceObject(BackgroundTexture);
        NewBrush.DrawAs = ESlateBrushDrawType::RoundedBox;
        NewBrush.ImageSize = FVector2D(BackgroundTexture->GetSizeX(), BackgroundTexture->GetSizeY());
        ImageBackground->SetBrush(NewBrush);
    }

    BTN_Pull->BackgroundTexture = ButtonPullTexture;
    BTN_Pull->Refresh();
    BTN_PullMulti->BackgroundTexture = ButtonPull10Texture;
    BTN_PullMulti->Refresh();
}

void UBannerWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Récupérer la référence au PlayerActor
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        PlayerActorRef = Cast<APlayerActor>(PC->GetPawn());
    }

    if (BTN_Pull)
    {
        BTN_Pull->OnCustomButtonClicked.AddDynamic(this, &UBannerWidget::HandlePullClicked);
    }

    if (BTN_PullMulti)
    {
        BTN_PullMulti->OnCustomButtonClicked.AddDynamic(this, &UBannerWidget::HandlePullMultiClicked);
    }
}

void UBannerWidget::HandlePullClicked()
{
    FName PulledCharacter = PerformSinglePull();
    if (PulledCharacter == NAME_None)
    {
        return;
    }

    TArray<FName> PullResults;
    PullResults.Add(PulledCharacter);
    
    // Ajouter au PlayerActor au lieu de sauvegarder
    AddPulledWorkersToPlayer(PullResults);

    if (ParentGachaWidget)
    {
        ParentGachaWidget->ShowPullResultsWithShowcase(PullResults);
        ParentGachaWidget->BTN_Back->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UBannerWidget::HandlePullMultiClicked()
{
    if (!CharacterDataTable)
    {
        return;
    }

    TArray<FName> PullResults;
    for (int i = 0; i < 10; ++i)
    {
        FName PulledCharacter = PerformSinglePull();
        if (PulledCharacter != NAME_None)
        {
            PullResults.Add(PulledCharacter);
        }
    }

    // Ajouter au PlayerActor au lieu de sauvegarder
    AddPulledWorkersToPlayer(PullResults);

    if (ParentGachaWidget)
    {
        ParentGachaWidget->ShowPullResultsWithShowcase(PullResults);
        ParentGachaWidget->BTN_Back->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UBannerWidget::AddPulledWorkersToPlayer(const TArray<FName>& PullResults)
{
    if (!PlayerActorRef)
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerActorRef is null in AddPulledWorkersToPlayer"));
        return;
    }

    // Pour chaque worker tiré, l'ajouter à l'inventaire du joueur
    // Le système gérera automatiquement le spawn et l'ajout de stars
    for (const FName& CharacterID : PullResults)
    {
        PlayerActorRef->AddWorkerToInventory(CharacterID, 1);
    }
}

FName UBannerWidget::PerformSinglePull()
{
    if (!CharacterDataTable)
        return NAME_None;

    TArray<FName> RowNames = CharacterDataTable->GetRowNames();
    float TotalWeightedDropRate = 0.f;
    TArray<FName> ValidCharacterRowNames;

    for (const FName& RowName : RowNames)
    {
        FCharacterStructure* Data = CharacterDataTable->FindRow<FCharacterStructure>(RowName, TEXT(""));
        if (!Data)
        {
            continue;
        }

        float* RarityDropRate = DropRatesByRarity.Find(Data->Rarity);
        float Weight = RarityDropRate ? *RarityDropRate : 0.f;

        if (Weight > 0.f)
        {
            TotalWeightedDropRate += Weight;
            ValidCharacterRowNames.Add(RowName);
        }
    }

    if (TotalWeightedDropRate <= 0.f || ValidCharacterRowNames.Num() == 0)
        return NAME_None;

    float RandomPoint = FMath::FRandRange(0.f, TotalWeightedDropRate);
    float Accumulated = 0.f;

    for (const FName& RowName : ValidCharacterRowNames)
    {
        FCharacterStructure* CharData = CharacterDataTable->FindRow<FCharacterStructure>(RowName, TEXT(""));
        if (!CharData)
        {
            continue;
        }

        float* RarityDropRate = DropRatesByRarity.Find(CharData->Rarity);
        float Weight = RarityDropRate ? *RarityDropRate : 0.f;
        Accumulated += Weight;

        if (RandomPoint <= Accumulated)
        {
            return RowName;
        }
    }

    return NAME_None;
}