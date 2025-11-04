#include "GachaInventoryItemWidget.h"
#include "Components/TextBlock.h"

void UGachaInventoryItemWidget::InitializeWithData(const FCharacterStructure& CharacterData, const FCharacterProgress& Progress)
{
	if (NameText)
	{
		NameText->SetText(FText::FromString(CharacterData.Name));
	}

	if (RarityText)
	{
		FString RarityString;
		switch (CharacterData.Rarity)
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

	if (StarText)
	{
		FString StarsText = FString::Printf(TEXT("%d / %d ★"), Progress.StarCount, CharacterData.StarMax);
		StarText->SetText(FText::FromString(StarsText));
	}

	if (StatYoutubeText)
	{
		StatYoutubeText->SetText(FText::AsNumber(Progress.StatYoutube));
	}

	if (StatTikTokText)
	{
		StatTikTokText->SetText(FText::AsNumber(Progress.StatTikTok));
	}
}
