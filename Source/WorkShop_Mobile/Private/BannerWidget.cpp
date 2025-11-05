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
    if (!CharacterDataTable)
    {
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CharacterDataTable non valide."));
        return;
    }

    TArray<FName> RowNames = CharacterDataTable->GetRowNames();

    float TotalWeightedDropRate = 0.f;
    TArray<FName> ValidCharacterRowNames;

    for (const FName& RowName : RowNames)
    {
        FCharacterStructure* Data = CharacterDataTable->FindRow<FCharacterStructure>(RowName, TEXT(""));
        if (!Data)
        {
            if (GEngine)
                GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Data character non trouvé pour %s"), *RowName.ToString()));
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
    {
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Aucun personnage valide ou taux total nul."));
        return;
    }

    float RandomPoint = FMath::FRandRange(0.f, TotalWeightedDropRate);
    float Accumulated = 0.f;

    FName SelectedCharacterRowName = NAME_None;
    FCharacterStructure* SelectedCharacter = nullptr;

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
            SelectedCharacterRowName = RowName;
            SelectedCharacter = CharData;
            break;
        }
    }

    if (!SelectedCharacter)
    {
        if (GEngine)
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Aucun personnage sélectionné."));
        return;
    }

    if (GEngine)
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Personnage sélectionné : %s"), *SelectedCharacterRowName.ToString()));

    // Ajoute une nouvelle entrée dans CharactersInventory (pas de fusion)
    FCharacterProgress NewProgress;
    NewProgress.CharacterID = SelectedCharacterRowName;
    NewProgress.StarCount = 1; // Toujours 1 étoile

    CharactersInventory.Add(NewProgress);

    SaveProgress();

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Après pull, CharactersInventory:"));
        for (const FCharacterProgress& Elem : CharactersInventory)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("%s - Etoiles: %d"), *Elem.CharacterID.ToString(), Elem.StarCount));
        }
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
        SaveGameInstance->SavedCharactersArray = CharactersInventory;  // Utilise un tableau dans ta classe de sauvegarde
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

void UBannerWidget::HandlePullMultiClicked()
{
    if (!CharacterDataTable)
        return;

    for (int i = 0; i < 10; i++)
    {
        HandlePullClicked();
    }
}