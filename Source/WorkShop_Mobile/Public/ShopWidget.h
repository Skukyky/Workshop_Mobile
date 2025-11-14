// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Input/Reply.h"
#include "PlayerActor.h"
#include "Components/WidgetSwitcher.h"
#include "ShopWidget.generated.h"

/**
 * 
 */
UCLASS()
class WORKSHOP_MOBILE_API UShopWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlayerActor* PlayerActor;

	//---------------ShopTab---------------
	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Boutique;
	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Offers;

	UFUNCTION()
	void OnBoutique_Clicked();
	UFUNCTION()
	void OnOffers_Clicked();
	
	//---------------LeaveBTN---------------
	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Leave;

	UFUNCTION()
	void OnLeave_Clicked();

	//---------------Shop---------------
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* BoutiqueSwitcher;

	//---------------ShopPanel---------------
	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Shop_1;
	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Shop_2;
	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Shop_3;
	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Shop_4;
	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Shop_5;
	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Shop_6;

	UFUNCTION()
	void OnShop_1_Clicked();
	UFUNCTION()
	void OnShop_2_Clicked();
	UFUNCTION()
	void OnShop_3_Clicked();
	UFUNCTION()
	void OnShop_4_Clicked();
	UFUNCTION()
	void OnShop_5_Clicked();
	UFUNCTION()
	void OnShop_6_Clicked();

	//---------------OffersPanel---------------
	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Offer_1;
	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Offer_2;
	UPROPERTY(meta = (BindWidget))
	UButton* BTN_Offer_3;

	UFUNCTION()
	void OnOffer_1_Clicked();
	UFUNCTION()
	void OnOffer_2_Clicked();
	UFUNCTION()
	void OnOffer_3_Clicked();
};