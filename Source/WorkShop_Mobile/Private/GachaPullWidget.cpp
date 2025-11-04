#include "GachaPullWidget.h"
#include "CharacterStructure.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "GachaSaveGame.h" 

void UGachaPullWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (BTN_Pull)
    {
        BTN_Pull->OnClicked.AddDynamic(this, &UGachaPullWidget::OnPullButtonClicked);
    }

    LoadProgress();
}

void UGachaPullWidget::OnPullButtonClicked()
{
    if (!CharacterDataTable) return;

    TArray<FName> RowNames = CharacterDataTable->GetRowNames();

    // Calculer la somme totale des DropRate
    float TotalDropRate = 0.f;
    TArray<FCharacterStructure*> CharactersData;

    for (const FName& RowName : RowNames)
    {
        FCharacterStructure* Data = CharacterDataTable->FindRow<FCharacterStructure>(RowName, TEXT(""));
        if (Data)
        {
            CharactersData.Add(Data);
            TotalDropRate += Data->DropRate;
        }
    }

    if (TotalDropRate <= 0.f || CharactersData.Num() == 0) return;

    // Tirage pondéré
    float RandomPoint = FMath::FRandRange(0.f, TotalDropRate);
    float Accumulated = 0.f;
    FCharacterStructure* SelectedCharacter = nullptr;
    FName SelectedRowName; // Pour la clé dans la map

    for (int32 i = 0; i < CharactersData.Num(); i++)
    {
        Accumulated += CharactersData[i]->DropRate;
        if (RandomPoint <= Accumulated)
        {
            SelectedCharacter = CharactersData[i];
            SelectedRowName = RowNames[i];
            break;
        }
    }

    if (!SelectedCharacter) return;

    if (CharactersProgress.Contains(SelectedRowName))
    {
        FCharacterProgress& Progress = CharactersProgress[SelectedRowName];
        if (Progress.StarCount < SelectedCharacter->StarMax)
        {
            Progress.StarCount++;
        }
        Progress.StatYoutube += SelectedCharacter->StatYoutube;
        Progress.StatTikTok += SelectedCharacter->StatTikTok;
    }
    else
    {
        FCharacterProgress NewProgress;
        NewProgress.CharacterRowName = SelectedRowName;
        NewProgress.StarCount = 1;
        NewProgress.StatYoutube = SelectedCharacter->StatYoutube;
        NewProgress.StatTikTok = SelectedCharacter->StatTikTok;
        CharactersProgress.Add(SelectedRowName, NewProgress);
    }

    SaveProgress();

    const FCharacterProgress& CurrentProgress = CharactersProgress[SelectedRowName];

    NameText->SetText(FText::FromString(SelectedCharacter->Name));
    StatYoutubeText->SetText(FText::AsNumber(CurrentProgress.StatYoutube));
    StatTikTokText->SetText(FText::AsNumber(CurrentProgress.StatTikTok));

    // Affiche étoiles dans StarText
    FString StarsText = FString::Printf(TEXT("%d / %d ★"), CurrentProgress.StarCount, SelectedCharacter->StarMax);
    if (StarText)
    {
        StarText->SetText(FText::FromString(StarsText));
    }

    FString TypeString = (SelectedCharacter->Type == ECharacterType::Youtube) ? TEXT("Youtube") : TEXT("TikTok");
    TypeText->SetText(FText::FromString(TypeString));

    // Garde RarityText pour afficher la rareté vraie
    FString RarityString;
    switch (SelectedCharacter->Rarity)
    {
        case ECharacterRarity::Commun:    RarityString = TEXT("Commun"); break;
        case ECharacterRarity::Rare:      RarityString = TEXT("Rare"); break;
        case ECharacterRarity::Epique:    RarityString = TEXT("Epique"); break;
        case ECharacterRarity::Legendary: RarityString = TEXT("Legendary"); break;
        case ECharacterRarity::Secret:    RarityString = TEXT("Secret"); break;
        default:                         RarityString = TEXT("Inconnu"); break;
    }
    RarityText->SetText(FText::FromString(RarityString));
}

void UGachaPullWidget::SaveProgress()
{
    UGachaSaveGame* SaveGameInstance = Cast<UGachaSaveGame>(UGameplayStatics::CreateSaveGameObject(UGachaSaveGame::StaticClass()));

    if (SaveGameInstance)
    {
        SaveGameInstance->SavedCharacters = CharactersProgress;
        UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("GachaSaveSlot"), 0);
    }
}

void UGachaPullWidget::LoadProgress()
{
    if (UGameplayStatics::DoesSaveGameExist(TEXT("GachaSaveSlot"), 0))
    {
        UGachaSaveGame* LoadedGame = Cast<UGachaSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("GachaSaveSlot"), 0));

        if (LoadedGame)
        {
            CharactersProgress = LoadedGame->SavedCharacters;
        }
    }
}
