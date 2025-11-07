// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkRoomSettingWidget.h"
#include "BTNCustomWidget.h"
#include "RoomWorking.h"
#include "Worker.h"

void UWorkRoomSettingWidget::OnGoldClicked()
{
	RoomWorking->SendMoneyToPlayer();
}

void UWorkRoomSettingWidget::OnUpgradeCliqued()
{
	RoomWorking->CanUpgradeWithMoney();
}

void UWorkRoomSettingWidget::OnExitClicked()
{
	RemoveFromParent();
}

void UWorkRoomSettingWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
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
			UE_LOG(LogTemp, Display, TEXT("Work Room Setting n°%d"), i);
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
	}
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
