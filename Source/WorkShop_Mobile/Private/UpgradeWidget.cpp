// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeWidget.h"

void UUpgradeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (Close)
	{
		Close->OnReleased.AddDynamic(this, &UUpgradeWidget::ClickClose);
	}
}

void UUpgradeWidget::ClickClose()
{
	RemoveFromParent();
}
