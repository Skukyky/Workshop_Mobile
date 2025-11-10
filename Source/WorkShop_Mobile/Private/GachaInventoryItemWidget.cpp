#include "GachaInventoryItemWidget.h"

#include "BTNCustomWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UGachaInventoryItemWidget::InitializeWithData(const FCharacterStructure& CharacterData, const FCharacterProgress& Progress, FName InCharacterID)
{
	CharacterID = InCharacterID;
	ProgressRef = Progress;
	// Ton code existant d'initialisation
	if (NameText)
	{
		NameText->SetText(FText::FromString(CharacterData.Name));
	}
	
	if (StarText)
	{
		FString StarsText = FString::Printf(TEXT("%d / %d ★"), Progress.StarCount, CharacterData.StarMax);
		StarText->SetText(FText::FromString(StarsText));
	}
	

	if (CharacterPDP)
	{
		FSlateBrush NewBrush;
		NewBrush.SetResourceObject(CharacterData.Photo);
		NewBrush.ImageSize = FVector2D(CharacterData.Photo->GetSizeX(), CharacterData.Photo->GetSizeY());
		CharacterPDP->SetBrush(NewBrush);
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
