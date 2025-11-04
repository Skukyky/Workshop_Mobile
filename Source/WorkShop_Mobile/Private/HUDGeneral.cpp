// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDGeneral.h"



void UHUDGeneral::CloseOpenPanel_Implementation()
{
}

void UHUDGeneral::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (OptionAchat)
	{
		OptionAchat->OnReleased.AddDynamic(this, &UHUDGeneral::CloseOpenPanel);
	}
	if (ReserveButton)
	{
		ReserveButton->OnReleased.AddDynamic(this, &UHUDGeneral::ClickReserveButton);
	}
	if (ShopButton)
	{
		ShopButton->OnReleased.AddDynamic(this, &UHUDGeneral::ClickShopButton);
	}
	if (UpgradeButton)
	{
		UpgradeButton->OnReleased.AddDynamic(this, &UHUDGeneral::ClickUpgradeButton);
	}
	if (OptionButton)
	{
		OptionButton->OnReleased.AddDynamic(this, &UHUDGeneral::ClickOptionButton);
	}
}

void UHUDGeneral::ClickReserveButton()
{
	EmployerWidgetRef = CreateWidget<UUserWidget>(GetWorld(), WidgetEmployerReference);
	if (EmployerWidgetRef) EmployerWidgetRef->AddToViewport();
}

void UHUDGeneral::ClickShopButton()
{
	ShopWidgetRef = CreateWidget<UUserWidget>(GetWorld(), WidgetShopReference);
	if (ShopWidgetRef) ShopWidgetRef->AddToViewport();
}

void UHUDGeneral::ClickUpgradeButton()
{
	UpgradeWidgetRef = CreateWidget<UUserWidget>(GetWorld(), WidgetUpgradeReference);
	if (UpgradeWidgetRef) UpgradeWidgetRef->AddToViewport();
}

void UHUDGeneral::ClickOptionButton()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Some debug message!"));	
}
