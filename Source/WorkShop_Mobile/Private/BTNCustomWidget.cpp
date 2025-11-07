#include "BTNCustomWidget.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

void UBTNCustomWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (BTN_Custom)
	{
		// Utilise AddUniqueDynamic pour éviter d'ajouter plusieurs fois la même liaison
		BTN_Custom->OnClicked.AddUniqueDynamic(this, &UBTNCustomWidget::OnCustomButtonClickedHandler);

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
			
			DesiredSize->SetHeightOverride(BackgroundTexture->GetSizeX());
			DesiredSize->SetWidthOverride(BackgroundTexture->GetSizeY());
		}
	}
}

void UBTNCustomWidget::ChangeDesiredSize(FVector2D SizeMax)
{
	float SizeToScale = FMath::Min(SizeMax.X, SizeMax.Y);
	if (BackgroundTexture)
	{
		float Scaling = BackgroundTexture->GetSizeY() / SizeToScale;
		if (SizeMax.X < SizeMax.Y)
		{
			Scaling = BackgroundTexture->GetSizeX() / SizeToScale;
		}
		DesiredSize->SetHeightOverride(BackgroundTexture->GetSizeX()/Scaling);
		DesiredSize->SetWidthOverride(BackgroundTexture->GetSizeY()/Scaling);
	}
}


void UBTNCustomWidget::OnCustomButtonClickedHandler()
{
	OnCustomButtonClicked.Broadcast();
}
