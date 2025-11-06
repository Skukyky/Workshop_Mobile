#include "GachaInventoryItemWidget.h"

#include "BTNCustomWidget.h"
#include "Components/TextBlock.h"

void UGachaInventoryItemWidget::InitializeWithData(const FCharacterStructure& CharacterData, const FCharacterProgress& Progress, FName InCharacterID)
{
	CharacterID = InCharacterID;

	// Ton code existant d'initialisation
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
	if (TypeText)
	{
		FString TypeString;

		switch (CharacterData.Type)
		{
		case ECharacterType::Youtube:
			TypeString = TEXT("Youtube");
			break;
		case ECharacterType::TikTok:
			TypeString = TEXT("TikTok");
			break;
		default:
			TypeString = TEXT("Inconnu");
			break;
		}

		TypeText->SetText(FText::FromString(TypeString));
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

void UGachaInventoryItemWidget::OnItemClicked()
{
	if (OnItemSelected.IsBound())
	{
		OnItemSelected.Broadcast(this);
	}
}

void UGachaInventoryItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BTN_SelectionItem)
	{
		BTN_SelectionItem->OnCustomButtonClicked.AddDynamic(this, &UGachaInventoryItemWidget::OnItemClicked);
	}
}
