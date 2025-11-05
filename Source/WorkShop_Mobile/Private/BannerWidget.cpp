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
    if (!CharacterDataTable) return;

    TArray<FName> RowNames = CharacterDataTable->GetRowNames();

    float TotalWeightedDropRate = 0.f;
    TArray<FCharacterStructure*> ValidCharacters;

    for (const FName& RowName : RowNames)
    {
       FCharacterStructure* Data = CharacterDataTable->FindRow<FCharacterStructure>(RowName, TEXT(""));
       if (!Data) continue;

       float* RarityDropRate = DropRatesByRarity.Find(Data->Rarity);
       float Weight = RarityDropRate ? *RarityDropRate : 0.f;

       if (Weight > 0.f)
       {
          TotalWeightedDropRate += Weight;
          ValidCharacters.Add(Data);
       }
    }

    if (TotalWeightedDropRate <= 0.f || ValidCharacters.Num() == 0) return;

    float RandomPoint = FMath::FRandRange(0.f, TotalWeightedDropRate);
    float Accumulated = 0.f;
    FCharacterStructure* SelectedCharacter = nullptr;

    for (FCharacterStructure* CharData : ValidCharacters)
    {
       float* RarityDropRate = DropRatesByRarity.Find(CharData->Rarity);
       float Weight = RarityDropRate ? *RarityDropRate : 0.f;

       Accumulated += Weight;
       if (RandomPoint <= Accumulated)
       {
          SelectedCharacter = CharData;
          break;
       }
    }

    if (!SelectedCharacter) return;

    // TODO: Mise à jour du progrès, UI, et autres actions avec SelectedCharacter

    SaveProgress();
}

void UBannerWidget::HandlePullMultiClicked()
{
    if (!CharacterDataTable) return;

    for (int i = 0; i < 10; i++)
    {
       HandlePullClicked();
    }
}

void UBannerWidget::SaveProgress()
{
    UGachaSaveGame* SaveGameInstance = Cast<UGachaSaveGame>(UGameplayStatics::CreateSaveGameObject(UGachaSaveGame::StaticClass()));

    if (SaveGameInstance)
    {
       SaveGameInstance->SavedCharacters = CharactersProgress;
       UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("GachaSaveSlot"), 0);
    }
}
