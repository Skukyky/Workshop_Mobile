// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GachaPullWidget.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class WORKSHOP_MOBILE_API UGachaPullWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Pull;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TypeText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* RarityText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StatYoutubeText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* StatTikTokText;
	
};
