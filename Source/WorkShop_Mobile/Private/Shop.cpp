// Fill out your copyright notice in the Description page of Project Settings.


#include "Shop.h"

void UShop::NativeConstruct()
{
	Super::NativeConstruct();
	if (Close)
	{
		Close->OnReleased.AddDynamic(this, &UShop::ClickClose);
	}
}

void UShop::ClickClose()
{
	RemoveFromParent();
}