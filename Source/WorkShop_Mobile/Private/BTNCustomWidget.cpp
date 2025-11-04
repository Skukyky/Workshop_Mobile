// Fill out your copyright notice in the Description page of Project Settings.


#include "BTNCustomWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UBTNCustomWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (BTN_Custom)
	{
		BTN_Custom->OnClicked.AddUniqueDynamic(this, &UBTNCustomWidget::OnBTNCustomClicked);
	}

	BTN_Custom->SetBackgroundColor(BackgroundColor);
	TextBTN->SetText(Text);
	
}

void UBTNCustomWidget::OnBTNCustomClicked()
{
	OnCustomButtonClicked.Broadcast();
}