// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDGeneral.h"

#include "GachaPullWidget.h"
#include "PlayerActor.h"
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
	GetWorld()->GetTimerManager().SetTimer(UpdateStartDelay,this,&UHUDGeneral::UpdateValue,3.0f,false);
}

void UHUDGeneral::ClickReserveButton()
{
	EmployerWidgetRef = CreateWidget<UUserWidget>(GetWorld(), WidgetEmployerReference);
	if (EmployerWidgetRef) EmployerWidgetRef->AddToViewport();
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		FInputModeUIOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true;
	}
}

void UHUDGeneral::ClickShopButton()
{
	ShopWidgetRef = CreateWidget<UUserWidget>(GetWorld(), WidgetShopReference);
	if (ShopWidgetRef) ShopWidgetRef->AddToViewport();
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		FInputModeUIOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true;
	}
}

void UHUDGeneral::ClickInvocationButton()
{
	InvocationWidgetRef = CreateWidget<UGachaPullWidget>(GetWorld(), WidgetInvocationReference);
	InvocationWidgetRef->PlayerREF = PlayerActorRef;
	if (InvocationWidgetRef) InvocationWidgetRef->AddToViewport();
	PlayerActorRef->ChangeVolumeMusic(true);
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		FInputModeUIOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true;
	}
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
	Option->SetVisibility(ESlateVisibility::Visible);
	VisiblityChange(false);
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		FInputModeUIOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true;
	}
}

void UHUDGeneral::ClickCloseOption()
{
	VisiblityChange(true);
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		FInputModeGameAndUI InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true;
	}
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
			if (BoolVibrationButton && VibrationDisableImage)
			{
				FButtonStyle ButtonStyle = BoolVibrationButton->GetStyle();
        
				FSlateBrush NewBrush;
				NewBrush.SetResourceObject(VibrationDisableImage);

				ButtonStyle.SetNormal(NewBrush);
				ButtonStyle.SetHovered(NewBrush);
				ButtonStyle.SetPressed(NewBrush);

				BoolVibrationButton->SetStyle(ButtonStyle);
			}
		}
		else
		{
			Vibration = true;
			PlayerController->bForceFeedbackEnabled = true;
			if (BoolVibrationButton && VibrationActiveImage)
			{
				FButtonStyle ButtonStyle = BoolVibrationButton->GetStyle();
        
				FSlateBrush NewBrush;
				NewBrush.SetResourceObject(VibrationActiveImage);

				ButtonStyle.SetNormal(NewBrush);
				ButtonStyle.SetHovered(NewBrush);
				ButtonStyle.SetPressed(NewBrush);

				BoolVibrationButton->SetStyle(ButtonStyle);
			}
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
		FString StringMoney = FString::SanitizeFloat(Count);
		MoneyText->SetText(FText::FromString(StringMoney));
	}
}

void UHUDGeneral::UpdateAbonnerText(int Count)
{
	if (AbonnerText)
	{
		FString StringAbonner = FString::SanitizeFloat(Count);
		AbonnerText->SetText(FText::FromString(StringAbonner));
	}
}

void UHUDGeneral::UpdateGemText(int Count)
{
	if (GemText)
	{
		FString StringGem = FString::SanitizeFloat(Count);
		GemText->SetText(FText::FromString(StringGem));
	}
}

void UHUDGeneral::UpdateFollowerText(int Count)
{
	if (FollowerText)
	{
		FString StringFollower = FString::SanitizeFloat(Count);
		FollowerText->SetText(FText::FromString(StringFollower));
	}
}

void UHUDGeneral::UpdateValue()
{
	UpdateMoneyText(PlayerActorRef->GetMoney());
	UpdateGemText(PlayerActorRef->GetGem());
	UpdateFollowerText(PlayerActorRef->GetFollower());
	UpdateAbonnerText(PlayerActorRef->GetPoolResource());
}
