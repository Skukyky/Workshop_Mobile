#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BTNCustomWidget.generated.h"

class USizeBox;
class UButton;
class UGachaInventoryWidget;
class UWorkRoomSettingWidget;
class UTextBlock;

/** Délégué multicast dynamique déclenché quand le bouton est cliqué */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCustomButtonClicked);

UCLASS()
class WORKSHOP_MOBILE_API UBTNCustomWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Custom;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBTN;

	UPROPERTY(meta = (BindWidget))
	USizeBox* DesiredSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor BackgroundColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Text = FText::FromString("");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D CustomHeight = FVector2D::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PourcentSize = 100;

	/** Événement déclenché lorsque le bouton est cliqué */
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCustomButtonClicked OnCustomButtonClicked;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* BackgroundTexture = nullptr;

	/** Fonction appelée quand le bouton est cliqué */
	UFUNCTION()
	void OnCustomButtonClickedHandler();

	virtual void NativePreConstruct() override;

	UFUNCTION()
	void ChangeDesiredSize(FVector2D SizeMax);

	UPROPERTY()
	bool IsWorkerAssignableButton = false;

	UPROPERTY()
	UWorkRoomSettingWidget* WorkRoomSettingWidget = nullptr;

	UPROPERTY()
	int Position;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGachaInventoryWidget> Inventory;

private:
	UPROPERTY()
	UGachaInventoryWidget* GachaInventoryWidget;
};
