// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "UpgradeWidget.generated.h"

/**
 * 
 */
UCLASS()
class WORKSHOP_MOBILE_API UUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct();
	
	UPROPERTY(meta = (BindWidget))
	UButton* Close;

	UFUNCTION()
	void ClickClose();
};
