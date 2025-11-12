// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Styling/SlateBrush.h"
#include "HUDGeneral.generated.h"

class UGachaPullWidget;
class APlayerActor;
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
	UButton* InvocationButton;

	UPROPERTY()
	UGachaPullWidget* InvocationWidgetRef;
	
	UPROPERTY(EditDefaultsOnly, Category = Widget)
	TSubclassOf<UGachaPullWidget> WidgetInvocationReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widget)
	APlayerActor* PlayerActorRef;
	
	UFUNCTION()
	void ClickInvocationButton();

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button Style")
	UTexture2D* VibrationActiveImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Button Style")
	UTexture2D* VibrationDisableImage;
	
	UFUNCTION()
	void ClickBoolVibrationButton();

	UPROPERTY(meta = (BindWidget))
	USlider* SoundEffectsSlider;

	UPROPERTY(meta = (BindWidget))
	USlider* MusicSlider;

	UFUNCTION()
	void OnSoundEffectsChanged(float Value);

	UFUNCTION()
	void OnMusicChanged(float Value);

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundMix* MasterSoundMix;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundClass* SoundEffectsSoundClass;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundClass* MusicSoundClass;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MoneyText;

	void UpdateMoneyText(int Count);
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AbonnerText;

	void UpdateAbonnerText(int Count);
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* GemText;

	void UpdateGemText(int Count);
};
