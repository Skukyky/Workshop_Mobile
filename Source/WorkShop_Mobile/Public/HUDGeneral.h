// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "HUDGeneral.generated.h"

/**
 * 
 */
UCLASS()
class WORKSHOP_MOBILE_API UHUDGeneral : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct();
	
	UPROPERTY(meta = (BindWidget))
	UButton* OptionAchat;

	UFUNCTION(BlueprintNativeEvent)
	void CloseOpenPanel();

	UPROPERTY(meta = (BindWidget))
	UButton* ReserveButton;

	UPROPERTY()
	UUserWidget* EmployerWidgetRef;
	
	UPROPERTY(EditDefaultsOnly, Category = Widget)
	TSubclassOf<UUserWidget> WidgetEmployerReference;
	
	UFUNCTION()
	void ClickReserveButton();
		
	UPROPERTY(meta = (BindWidget))
	UButton* ShopButton;

	UPROPERTY()
	UUserWidget* ShopWidgetRef;
	
	UPROPERTY(EditDefaultsOnly, Category = Widget)
	TSubclassOf<UUserWidget> WidgetShopReference;
	
	UFUNCTION()
	void ClickShopButton();
	
	UPROPERTY(meta = (BindWidget))
	UButton* UpgradeButton;

	UPROPERTY()
	UUserWidget* UpgradeWidgetRef;
	
	UPROPERTY(EditDefaultsOnly, Category = Widget)
	TSubclassOf<UUserWidget> WidgetUpgradeReference;
	
	UFUNCTION()
	void ClickUpgradeButton();

	UPROPERTY(meta = (BindWidget))
	UButton* OptionButton;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Option;

	bool VisibleOption = false;

	UFUNCTION(BlueprintNativeEvent)
	void VisiblityChange(bool Despawn);

	UFUNCTION(BlueprintCallable)
	void Despawn();
	
	UFUNCTION()
	void ClickOptionButton();

	UPROPERTY(meta = (BindWidget))
	UButton* CloseOption;

	UFUNCTION()
	void ClickCloseOption();

	UPROPERTY(meta = (BindWidget))
	UButton* CreditButton;

	UPROPERTY()
	UUserWidget* CreditWidgetRef;
	
	UPROPERTY(EditDefaultsOnly, Category = Widget)
	TSubclassOf<UUserWidget> WidgetCreditReference;
	
	UFUNCTION()
	void ClickCreditButton();

	UPROPERTY(meta = (BindWidget))
	UButton* BoolVibrationButton;

	bool Vibration = true;
	
	UFUNCTION()
	void ClickBoolVibrationButton();
};
