#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterStructure.h"
#include "CharacterProgress.h"
#include "GachaInventoryItemWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemSelected, UGachaInventoryItemWidget*, SelectedItem);

class UBTNCustomWidget;
class UTextBlock;
UCLASS()
class WORKSHOP_MOBILE_API UGachaInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UBTNCustomWidget* BTN_SelectionItem;
	
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
	
	UFUNCTION(BlueprintCallable)
	void InitializeWithData(const FCharacterStructure& CharacterData, const FCharacterProgress& Progress, FName InCharacterID);

	UFUNCTION()
	void OnItemClicked();

	UPROPERTY(BlueprintAssignable)
	FOnItemSelected OnItemSelected;

	UFUNCTION(BlueprintCallable)
	FName GetCharacterID() const { return CharacterID; }
	
	UPROPERTY()
	FName CharacterID = FName("");

	virtual void NativeConstruct() override;

	UPROPERTY()
	FCharacterProgress ProgressRef ;

	
};
