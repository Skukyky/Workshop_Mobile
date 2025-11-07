#pragma once

#include "CoreMinimal.h"
#include "CharacterProgress.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "GachaInventoryWidget.generated.h"

class APlayerActor;
class AGachaCharacterShowcase;
class UImage;
class UBTNCustomWidget;
class UScrollBox;
class UGachaInventoryItemWidget;

UCLASS()
class WORKSHOP_MOBILE_API UGachaInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UImage* CharacterImage;

	UPROPERTY(meta = (BindWidget))
	UImage* StatImage;

	UPROPERTY(meta = (BindWidget))
	UBTNCustomWidget* BTN_LostFocus;

	UPROPERTY(meta = (BindWidget))
	UBTNCustomWidget* BTN_Assign;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* InventoryScrollBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gacha")
	UDataTable* CharacterDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gacha")
	TSubclassOf<UGachaInventoryItemWidget> ItemWidgetClass;
    
	// Mis à jour pour accepter un tableau au lieu d'une map
	void PopulateInventory(const TArray<FCharacterProgress>& CharactersInventory);

	UFUNCTION()
	void OnLostFocusClicked();

	UFUNCTION()
	void OnAssignClicked();

	UPROPERTY()
	AGachaCharacterShowcase* CurrentCharacterShowcase = nullptr;

	// Classe à spawn (exposée pour assigner dans Blueprint)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gacha")
	TSubclassOf<AGachaCharacterShowcase> CharacterShowcaseActorClass;

	UPROPERTY()
	UBTNCustomWidget* AssignButtonReturn = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	UTexture2D* CV_Texture = nullptr;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY()
	bool IsAllReadyUse;

private:
	
	UPROPERTY()
	UGachaInventoryItemWidget* SelectedItemWidget = nullptr;

	UPROPERTY()
	APlayerActor* PlayerActor = nullptr;
	
	UFUNCTION()
	void OnItemSelected(UGachaInventoryItemWidget* ClickedItem);
};
