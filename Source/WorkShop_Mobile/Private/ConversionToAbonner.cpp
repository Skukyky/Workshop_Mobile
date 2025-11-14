// Fill out your copyright notice in the Description page of Project Settings.


#include "ConversionToAbonner.h"
#include "PlayerActor.h"

void UConversionToAbonner::NativeConstruct()
{
	Super::NativeConstruct();
	
	APlayerController* PlayerController = GetOwningPlayer();
    if (PlayerController)
    {
        PlayerActorRef = Cast<APlayerActor>(PlayerController->GetPawn());
    }
	if (GemSlider)
	{
		GemSlider->OnValueChanged.AddDynamic(this, &UConversionToAbonner::OnGemChanged);
		GemSlider->SetMaxValue(PlayerActorRef->GetGem());
	}
	if (CancelButton)
	{
		CancelButton->OnReleased.AddDynamic(this, &UConversionToAbonner::CancelButtonClicked);
	}
	if (ConfirmButton)
	{
		ConfirmButton->OnReleased.AddDynamic(this, &UConversionToAbonner::ConfirmButtonClicked);
	}
	if (GemText)
	{
		UpdateGemText(PlayerActorRef->GetGem());
	}
}

void UConversionToAbonner::OnGemChanged(float Value)
{
	NeedToConvert = Value;
	if (GemNumberText)
	{
		FString StringAbonner = FString::FromInt(Value);
		GemNumberText->SetText(FText::FromString(StringAbonner));
	}
}

void UConversionToAbonner::CancelButtonClicked()
{
	RemoveFromParent();
}

void UConversionToAbonner::ConfirmButtonClicked()
{
	PlayerActorRef->ChangeGemToPoolRessource(NeedToConvert);
	RemoveFromParent();
}

void UConversionToAbonner::UpdateGemText(int Count)
{
	if (GemText)
	{
		FString StringGem = FString::SanitizeFloat(Count);
		GemText->SetText(FText::FromString(StringGem));
	}
}
