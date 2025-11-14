// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "ConversionToAbonner.generated.h"

/**
 * 
 */
class APlayerActor;

UCLASS()
class WORKSHOP_MOBILE_API UConversionToAbonner : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget")
	APlayerActor* PlayerActorRef = nullptr;
	
	UPROPERTY(meta = (BindWidget))
	USlider* GemSlider;

	UFUNCTION()
	void OnGemChanged(float Value);

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GemNumberText;
	
	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;

	UFUNCTION()
	void CancelButtonClicked();
	
	UPROPERTY(meta = (BindWidget))
	UButton* ConfirmButton;

	UFUNCTION()
	void ConfirmButtonClicked();

	int NeedToConvert = 0;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* GemText;

	void UpdateGemText(int Count);
};
