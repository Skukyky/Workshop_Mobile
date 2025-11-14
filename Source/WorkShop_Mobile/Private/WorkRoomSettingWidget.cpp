// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkRoomSettingWidget.h"
#include "BTNCustomWidget.h"
#include "RoomWorking.h"
#include "Worker.h"

#include "Kismet/GameplayStatics.h"

void UWorkRoomSettingWidget::OnGoldClicked()
{
	RoomWorking->SendMoneyToPlayer();
	ActualiseMoney();
}

void UWorkRoomSettingWidget::ActualiseMoney()
{
	MoneySecond = 0;
	FollowerSecond = 0;
	for (int i = 0; i < RoomWorking->Workers.Num(); i++)
	{
		if (RoomWorking->Workers[i].Worker != nullptr)
		{
			MoneySecond += RoomWorking->Workers[i].Worker->MoneyPerWorkWithBonus;
			FollowerSecond += RoomWorking->Workers[i].Worker->MoneyPerWorkWithBonus/3;
		}
	}
	FString StringMoney = FString::SanitizeFloat(MoneySecond);
	FString StringFollower = FString::SanitizeFloat(FollowerSecond);
	FText Text = FText::Format(NSLOCTEXT("","","Money/s: {0} \n Follower/s : {1}"), FText::FromString(StringMoney), FText::FromString(StringFollower));
	GoldPerSecond->SetText(Text);
	
	FString StringAllMoneyStored = FString::SanitizeFloat(RoomWorking->CurrentMoneyInStock);
	FText TextAllMoney = FText::Format(NSLOCTEXT("","","All Money Stocked:\n {0}"), FText::FromString(StringAllMoneyStored));
	AllGoldStocked->SetText(TextAllMoney);

	FString StringMoneyUpgrade = FString::SanitizeFloat(RoomWorking->StatPerLevel[RoomWorking->LevelRoom].RequiredMoneyForUpgrade);
	FString StringFollowerUpgrade = FString::SanitizeFloat(RoomWorking->StatPerLevel[RoomWorking->LevelRoom].RequiredFollowerForNextUpgrade);
	FText TextUpgrade = FText::Format(NSLOCTEXT("","","Need Money : {0}\n Need Follower : {1}"), FText::FromString(StringMoneyUpgrade), FText::FromString(StringFollowerUpgrade));
	NeedGold->SetText(TextUpgrade);
	if (RoomWorking->CanUpgradeWithMoney())
	{
		UpgradeGold->BackgroundColor = FLinearColor(0.498264f, 0.498264f, 0.498264f, 1.0f);
	}
	else
	{
		UpgradeGold->BackgroundColor = FLinearColor(1.000000,1.000000,1.000000,1.000000);
	}
}


void UWorkRoomSettingWidget::OnExitClicked()
{
	ActualiseMoney();
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true;
	}
	RemoveFromParent();
}

void UWorkRoomSettingWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetIsFocusable(true);
	Refresh();
}

void UWorkRoomSettingWidget::Refresh()
{
	if (RoomWorking)
	{
		UE_LOG(LogTemp, Display, TEXT("Work Room Setting for %d"),RoomWorking->Workers.Num());
		for (int i = 0; i<=RoomWorking->Workers.Num()-1; i++)
		{
			UTexture2D* BackgroundTexture = BackgroundTextureIsEmpty;
			if (RoomWorking->Workers[i].Worker != nullptr)
			{
				BackgroundTexture = RoomWorking->Workers[i].Worker->GetMyCharacterStructure().Photo;
			}
			if (CustomButtonForWorker.Num() -1 < i)
			{
				UBTNCustomWidget* NewCustomButton = CreateWidget<UBTNCustomWidget>(this, Button);
				NewCustomButton->BackgroundTexture = BackgroundTexture;
				NewCustomButton->Text = FText::FromString("");
				NewCustomButton->IsWorkerAssignableButton = true;
				NewCustomButton->Position = i ;
				NewCustomButton->WorkRoomSettingWidget = this;
				//NewCustomButton->ChangeDesiredSize(FVector2D)
				NewCustomButton->PourcentSize = 80;
				int Row = i%2;
				int Column = i/2;
				GridPanel->AddChildToUniformGrid(NewCustomButton,Row,Column);
				CustomButtonForWorker.Add(NewCustomButton);
			}
			else
			{
				CustomButtonForWorker[i]->BackgroundTexture = BackgroundTexture;
				CustomButtonForWorker[i]->Refresh();
				RefreshStat();
			}
		}
	}
}

void UWorkRoomSettingWidget::RefreshStat()
{
	if (RoomWorking)
	{
		ActualiseMoney();
	}
}



void UWorkRoomSettingWidget::OnUpgradeCliqued()
{
	RoomWorking->Upgrading();
}


void UWorkRoomSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		FInputModeUIOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true;
	}
	SetFocus();
	
	if (GetGold)
	{
		GetGold->OnCustomButtonClicked.AddDynamic(this,&UWorkRoomSettingWidget::OnGoldClicked);
	}
	if (UpgradeGold)
	{
		UpgradeGold->OnCustomButtonClicked.AddDynamic(this,&UWorkRoomSettingWidget::OnUpgradeCliqued);
	}
	if (Exit)
	{
		Exit->OnCustomButtonClicked.AddDynamic(this,&UWorkRoomSettingWidget::OnExitClicked);
	}
	RefreshStat();
	
	
}

void UWorkRoomSettingWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
