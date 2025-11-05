#include "BannerWidget.h"
#include "GachaSaveGame.h"
#include "Kismet/GameplayStatics.h"

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
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Aucun personnage sélectionné."));
        return;
    }

    TArray<FName> PullResults;
    PullResults.Add(PulledCharacter);

    MergePullResults(PullResults);
    SaveProgress();

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
            FString::Printf(TEXT("Personnage tiré : %s"), *PulledCharacter.ToString()));

        for (const FCharacterProgress& Elem : CharactersInventory)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
                FString::Printf(TEXT("%s - Etoiles: %d"), *Elem.CharacterID.ToString(), Elem.StarCount));
        }
    }
}

void UBannerWidget::HandlePullMultiClicked()
{
    if (!CharacterDataTable)
        return;

    TArray<FName> PullResults;
    for (int i = 0; i < 10; i++)
    {
        FName PulledCharacter = PerformSinglePull();
        if (PulledCharacter != NAME_None)
            PullResults.Add(PulledCharacter);
    }

    MergePullResults(PullResults);
    SaveProgress();

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Pull multi effectué."));

        for (const FCharacterProgress& Elem : CharactersInventory)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
                FString::Printf(TEXT("%s - Etoiles: %d"), *Elem.CharacterID.ToString(), Elem.StarCount));
        }
    }
}

void UBannerWidget::SaveProgress()
{
    UGachaSaveGame* SaveGameInstance = nullptr;

    // Charger sauvegarde existante
    if (UGameplayStatics::DoesSaveGameExist(TEXT("GachaSaveSlot"), 0))
    {
        SaveGameInstance = Cast<UGachaSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("GachaSaveSlot"), 0));
    }

    // Sinon créer une nouvelle sauvegarde
    if (!SaveGameInstance)
    {
        SaveGameInstance = Cast<UGachaSaveGame>(UGameplayStatics::CreateSaveGameObject(UGachaSaveGame::StaticClass()));
    }

    if (SaveGameInstance)
    {
        // Fusionner entre sauvegarde existante & nouvel inventaire accumulé
        TMap<FName, int32> AggregatedStars;

        // 1) ajouter sauvegarde courante
        for (const FCharacterProgress& Progress : SaveGameInstance->SavedCharactersArray)
        {
            AggregatedStars.FindOrAdd(Progress.CharacterID) += Progress.StarCount;
        }

        // 2) ajouter CharactersInventory (nouvel inventaire local)
        for (const FCharacterProgress& Progress : CharactersInventory)
        {
            AggregatedStars.FindOrAdd(Progress.CharacterID) += Progress.StarCount;
        }

        // Limite étoiles max à définir
        const int32 MaxStarCount = 5;

        // Réconstruire inventaire avec fusion et limite
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

        // Vide nouvel inventaire local puisque sauvegarde est mise à jour
        CharactersInventory.Empty();

        // Sauvegarde finale
        bool bSuccess = UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("GachaSaveSlot"), 0);

        if (bSuccess)
        {
            if (GEngine)
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Progression sauvegardée avec succès."));
        }
        else
        {
            if (GEngine)
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Erreur lors de la sauvegarde de la progression."));
        }
    }
    else
    {
        if (GEngine)
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
    // Map temporaire pour fusionner
    TMap<FName, int32> AggregatedStars;

    // Fusion avec inventaire actuel
    for (const FCharacterProgress& Progress : CharactersInventory)
    {
        AggregatedStars.FindOrAdd(Progress.CharacterID) += Progress.StarCount;
    }

    // Ajouter résultats du pull (1 étoile chacun)
    for (const FName& PulledID : PullResults)
    {
        AggregatedStars.FindOrAdd(PulledID) += 1;
    }

    const int32 MaxStarCount = 5;

    // Reconstruire CharactersInventory fusionné avec max 5 étoiles par entrée
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
