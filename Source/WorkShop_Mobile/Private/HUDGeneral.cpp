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
	if (CloseOption)
	{
		CloseOption->OnReleased.AddDynamic(this, &UHUDGeneral::ClickCloseOption);
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

void UHUDGeneral::VisiblityChange_Implementation(bool Despawn)
{
}

void UHUDGeneral::Despawn()
{
	Option->SetVisibility(ESlateVisibility::Hidden);
}

void UHUDGeneral::ClickOptionButton()
{
	if (!VisibleOption)
	{
		VisibleOption = true;
		Option->SetVisibility(ESlateVisibility::Visible);
		VisiblityChange(!VisibleOption);
	}
	else
	{
		VisibleOption = false;
		VisiblityChange(!VisibleOption);
	}
}

void UHUDGeneral::ClickCloseOption()
{
	VisibleOption = false;
	VisiblityChange(!VisibleOption);
}
