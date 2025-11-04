// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BTNCustomWidget.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */
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

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FLinearColor BackgroundColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Text = FText::FromString("");

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCustomButtonClicked OnCustomButtonClicked;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* BackgroundTexture = nullptr;
	UFUNCTION()
	void OnBTNCustomClicked();
	virtual void NativePreConstruct() override;
	
};
