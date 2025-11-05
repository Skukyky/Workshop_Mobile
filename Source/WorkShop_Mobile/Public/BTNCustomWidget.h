#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BTNCustomWidget.generated.h"

class UButton;
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor BackgroundColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Text = FText::FromString("");

	/** Événement déclenché lorsque le bouton est cliqué */
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCustomButtonClicked OnCustomButtonClicked;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* BackgroundTexture = nullptr;

	/** Fonction appelée quand le bouton est cliqué */
	UFUNCTION()
	void OnCustomButtonClickedHandler();

	virtual void NativePreConstruct() override;

};
