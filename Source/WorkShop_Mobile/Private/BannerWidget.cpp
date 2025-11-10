#include "BannerWidget.h"
#include "GachaSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "GachaPullWidget.h"
#include "Components/Image.h"

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
}

void UBannerWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
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

    MergePullResults(PullResults);
    SaveProgress();

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

    MergePullResults(PullResults);
    SaveProgress();

    if (ParentGachaWidget)
    {
        ParentGachaWidget->ShowPullResultsWithShowcase(PullResults);
        ParentGachaWidget->BTN_Back->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UBannerWidget::SaveProgress()
{
    UGachaSaveGame* SaveGameInstance = nullptr;

    if (UGameplayStatics::DoesSaveGameExist(TEXT("GachaSaveSlot"), 0))
    {
        SaveGameInstance = Cast<UGachaSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("GachaSaveSlot"), 0));
    }

    if (!SaveGameInstance)
    {
        SaveGameInstance = Cast<UGachaSaveGame>(UGameplayStatics::CreateSaveGameObject(UGachaSaveGame::StaticClass()));
    }

    if (SaveGameInstance)
    {
        TMap<FName, int32> AggregatedStars;

        for (const FCharacterProgress& Progress : SaveGameInstance->SavedCharactersArray)
        {
            AggregatedStars.FindOrAdd(Progress.CharacterID) += Progress.StarCount;
        }

        for (const FCharacterProgress& Progress : CharactersInventory)
        {
            AggregatedStars.FindOrAdd(Progress.CharacterID) += Progress.StarCount;
        }

        const int32 MaxStarCount = 5;

        SaveGameInstance->SavedCharactersArray.Empty();
        for (auto& Elem : AggregatedStars)
        {
            int32 TotalStars = Elem.Value;
            const FName& CharID = Elem.Key;

            while (TotalStars > 0)
            {
                int32 CurStars = FMath::Min(TotalStars, MaxStarCount);

                FCharacterProgress NewProgress;
                NewProgress.CharacterID = CharID;
                NewProgress.StarCount = CurStars;

                SaveGameInstance->SavedCharactersArray.Add(NewProgress);

                TotalStars -= CurStars;
            }
        }

        CharactersInventory.Empty();

        UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("GachaSaveSlot"), 0);
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

void UBannerWidget::MergePullResults(const TArray<FName>& PullResults)
{
    TMap<FName, int32> AggregatedStars;

    for (const FCharacterProgress& Progress : CharactersInventory)
    {
        AggregatedStars.FindOrAdd(Progress.CharacterID) += Progress.StarCount;
    }

    for (const FName& PulledID : PullResults)
    {
        AggregatedStars.FindOrAdd(PulledID) += 1;
    }

    const int32 MaxStarCount = 5;

    CharactersInventory.Empty();
    for (auto& Elem : AggregatedStars)
    {
        int32 TotalStars = Elem.Value;
        const FName& CharID = Elem.Key;

        while (TotalStars > 0)
        {
            int32 CurStars = FMath::Min(TotalStars, MaxStarCount);

            FCharacterProgress NewProgress;
            NewProgress.CharacterID = CharID;
            NewProgress.StarCount = CurStars;

            CharactersInventory.Add(NewProgress);

            TotalStars -= CurStars;
        }
    }
}
