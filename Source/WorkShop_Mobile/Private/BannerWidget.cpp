#include "BannerWidget.h"
#include "GachaSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "GachaPullWidget.h"
#include "Engine/Engine.h" // Pour GEngine

void UBannerWidget::SetParentGachaWidget(UGachaPullWidget* Parent)
{
    ParentGachaWidget = Parent;
    if (GEngine && !Parent)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ParentGachaWidget est null dans BannerWidget"));
    }
}

void UBannerWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (BTN_Pull)
    {
        BTN_Pull->OnCustomButtonClicked.AddDynamic(this, &UBannerWidget::HandlePullClicked);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Bouton Pull bindé"));
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Erreur : BTN_Pull non bindé"));
    }

    if (BTN_PullMulti)
    {
        BTN_PullMulti->OnCustomButtonClicked.AddDynamic(this, &UBannerWidget::HandlePullMultiClicked);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Bouton PullMulti bindé"));
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Erreur : BTN_PullMulti non bindé"));
    }
}

void UBannerWidget::HandlePullClicked()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("HandlePullClicked appelé"));

    FName PulledCharacter = PerformSinglePull();
    if (PulledCharacter == NAME_None)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Aucun personnage sélectionné lors du pull."));
        return;
    }

    TArray<FName> PullResults;
    PullResults.Add(PulledCharacter);

    MergePullResults(PullResults);
    SaveProgress();

    if (ParentGachaWidget)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Appel ShowPullResultsWithShowcase depuis BannerWidget"));
        ParentGachaWidget->ShowPullResultsWithShowcase(PullResults);
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ParentGachaWidget est null !"));
    }
}

void UBannerWidget::HandlePullMultiClicked()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("HandlePullMultiClicked appelé"));

    if (!CharacterDataTable)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CharacterDataTable est null"));
        return;
    }

    TArray<FName> PullResults;
    for (int i = 0; i < 10; ++i)
    {
        FName PulledCharacter = PerformSinglePull();
        if (PulledCharacter != NAME_None)
            PullResults.Add(PulledCharacter);
    }

    MergePullResults(PullResults);
    SaveProgress();

    if (ParentGachaWidget)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Appel ShowPullResultsWithShowcase depuis BannerWidget (multi)"));
        ParentGachaWidget->ShowPullResultsWithShowcase(PullResults);
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ParentGachaWidget est null (multi) !"));
    }

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Pull multi effectué."));
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

        if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("GachaSaveSlot"), 0))
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Progression sauvegardée avec succès."));
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Erreur lors de la sauvegarde de la progression."));
        }
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Impossible de créer ou charger la sauvegarde."));
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
            continue;

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
            continue;

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
