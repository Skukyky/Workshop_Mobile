// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialWidget.h"
#include "BTNCustomWidget.h"

void UTutorialWidget::HandleClicked()
{
	if (TutorialNumber < TutorialTexts.Num())
	{
		if (TEXT_Tuto)
		{
			TEXT_Tuto->SetText(FText::FromString(TutorialTexts[TutorialNumber]));
		}
		TutorialNumber++;
	}
	else
	{
		RemoveFromParent();
	}
}

void UTutorialWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BTN_Click)
	{
		BTN_Click->OnCustomButtonClicked.AddDynamic(this, &UTutorialWidget::HandleClicked);
	}
}
