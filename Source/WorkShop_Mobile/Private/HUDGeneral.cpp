// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDGeneral.h"
#include "Kismet/GameplayStatics.h"


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
	if (InvocationButton)
	{
		InvocationButton->OnReleased.AddDynamic(this, &UHUDGeneral::ClickInvocationButton);
	}
	if (OptionButton)
	{
		OptionButton->OnReleased.AddDynamic(this, &UHUDGeneral::ClickOptionButton);
	}
	if (CloseOption)
	{
		CloseOption->OnReleased.AddDynamic(this, &UHUDGeneral::ClickCloseOption);
	}
	if (CreditButton)
	{
		CreditButton->OnReleased.AddDynamic(this, &UHUDGeneral::ClickCreditButton);
	}
	if (BoolVibrationButton)
	{
		BoolVibrationButton->OnReleased.AddDynamic(this, &UHUDGeneral::ClickBoolVibrationButton);
	}
	if (SoundEffectsSlider)
	{
		SoundEffectsSlider->OnValueChanged.AddDynamic(this, &UHUDGeneral::OnSoundEffectsChanged);
	}
	if (MusicSlider)
	{
		MusicSlider->OnValueChanged.AddDynamic(this, &UHUDGeneral::OnMusicChanged);
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

void UHUDGeneral::ClickInvocationButton()
{
	InvocationWidgetRef = CreateWidget<UUserWidget>(GetWorld(), WidgetInvocationReference);
	if (InvocationWidgetRef) InvocationWidgetRef->AddToViewport();
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

void UHUDGeneral::ClickCreditButton()
{
	CreditWidgetRef = CreateWidget<UUserWidget>(GetWorld(), WidgetCreditReference);
	if (CreditWidgetRef) CreditWidgetRef->AddToViewport();
}

void UHUDGeneral::ClickBoolVibrationButton()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController)
	{
		if (Vibration)
		{
			Vibration = false;
			PlayerController->bForceFeedbackEnabled = false;
		}
		else
		{
			Vibration = true;
			PlayerController->bForceFeedbackEnabled = true;
		}
	}
}

void UHUDGeneral::OnSoundEffectsChanged(float Value)
{
	if (MasterSoundMix && SoundEffectsSoundClass)
	{
		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MasterSoundMix, SoundEffectsSoundClass, Value, 1.0f, 0.0f);
		UGameplayStatics::PushSoundMixModifier(GetWorld(), MasterSoundMix);
		SoundEffectsSlider->SetValue(Value);
	}
}

void UHUDGeneral::OnMusicChanged(float Value)
{
	if (MasterSoundMix && MusicSoundClass)
	{
		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MasterSoundMix, MusicSoundClass, Value, 1.0f, 0.0f);
		UGameplayStatics::PushSoundMixModifier(GetWorld(), MasterSoundMix);
		MusicSlider->SetValue(Value);
	}
}

void UHUDGeneral::UpdateMoneyText(int Count)
{
	if (MoneyText)
	{
		MoneyText->SetText(FText::AsNumber(Count));
	}
}

void UHUDGeneral::UpdateAbonnerText(int Count)
{
	if (AbonnerText)
	{
		AbonnerText->SetText(FText::AsNumber(Count));
	}
}

void UHUDGeneral::UpdateGemText(int Count)
{
	if (GemText)
	{
		GemText->SetText(FText::AsNumber(Count));
	}
}
