#pragma once

#include "CoreMinimal.h"
#include "CharacterProgress.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "GachaInventoryWidget.generated.h"

class UScrollBox;
class UGachaInventoryItemWidget;

UCLASS()
class WORKSHOP_MOBILE_API UGachaInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UScrollBox* InventoryScrollBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gacha")
	UDataTable* CharacterDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Gacha")
	TSubclassOf<UGachaInventoryItemWidget> ItemWidgetClass;
    
	// Mis à jour pour accepter un tableau au lieu d'une map
	void PopulateInventory(const TArray<FCharacterProgress>& CharactersInventory);

protected:
	virtual void NativeConstruct() override;
};
