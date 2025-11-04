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
		
		BTN_Custom->SetBackgroundColor(BackgroundColor);
		
		if (TextBTN)
		{
			TextBTN->SetText(Text);
		}

		if (BackgroundTexture)
		{
		
			FSlateBrush NewBrush;
			NewBrush.SetResourceObject(BackgroundTexture);
			NewBrush.ImageSize = FVector2D(BackgroundTexture->GetSizeX(), BackgroundTexture->GetSizeY());

		
			FButtonStyle ButtonStyle = BTN_Custom->GetStyle();

			
			ButtonStyle.SetNormal(NewBrush);
			ButtonStyle.SetHovered(NewBrush);  
			ButtonStyle.SetPressed(NewBrush);  
			
			BTN_Custom->SetStyle(ButtonStyle);
		}
	}
}

void UBTNCustomWidget::OnBTNCustomClicked()
{
	OnCustomButtonClicked.Broadcast();
}