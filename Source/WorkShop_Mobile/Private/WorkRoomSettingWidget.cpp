// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkRoomSettingWidget.h"
#include "BTNCustomWidget.h"
#include "RoomWorking.h"
#include "Worker.h"

void UWorkRoomSettingWidget::OnGoldClicked()
{
	RoomWorking->SendMoneyToPlayer();
	ActualiseMoney();
}

void UWorkRoomSettingWidget::ActualiseMoney()
{
	MoneySecond = 0;
	for (int i = 0; i < RoomWorking->Workers.Num(); i++)
	{
		if (RoomWorking->Workers[i].Worker != nullptr)
		{
			MoneySecond += RoomWorking->Workers[i].Worker->MoneyPerWorkWithBonus;
			UE_LOG(LogTemp, Warning, TEXT("Worker %d: %f"), i, RoomWorking->Workers[i].Worker->MoneyPerWork);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Total MoneySecond: %f"), MoneySecond);
	FString StringMoney = FString::SanitizeFloat(MoneySecond);
	GoldPerSecond->SetText(FText::FromString(StringMoney));
}


void UWorkRoomSettingWidget::OnExitClicked()
{
	ActualiseMoney();
	RemoveFromParent();
}

void UWorkRoomSettingWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
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
				GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Red,"zqdqzzdqzdqzdqzdqzdqzdqdqzdqzdqzdqzd");
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
		
	}
}



void UWorkRoomSettingWidget::OnUpgradeCliqued()
{
	RoomWorking->CanUpgradeWithMoney();
}


void UWorkRoomSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Display, TEXT("Work Room"));
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
}

void UWorkRoomSettingWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	ActualiseMoney();
}
