#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterStructure.h"
#include "CharacterProgress.h"
#include "GachaInventoryItemWidget.generated.h"

class UTextBlock;
UCLASS()
class WORKSHOP_MOBILE_API UGachaInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RarityText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TypeText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StarText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StatYoutubeText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StatTikTokText;
	
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void InitializeWithData(const FCharacterStructure& CharacterData, const FCharacterProgress& Progress);
};
