// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkRoomSettingWidget.h"
#include "BTNCustomWidget.h"
#include "RoomWorking.h"
#include "Worker.h"

void UWorkRoomSettingWidget::OnGoldClicked()
{
	RoomWorking->SendMoneyToPlayer();
}

void UWorkRoomSettingWidget::OnGemClicked()
{
	RoomWorking->CanUpgradeWithGem();
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
}

void UWorkRoomSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Display, TEXT("Work Room"));
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
			//NewCustomButton->ChangeDesiredSize(FVector2D)
			int Row = i%2;
			int Column = i/2;
			GridPanel->AddChildToUniformGrid(NewCustomButton,Row,Column);
			CustomButtonForWorker.Add(NewCustomButton);
		}
	}
	if (GetGold)
	{
		GetGold->OnCustomButtonClicked.AddDynamic(this,&UWorkRoomSettingWidget::OnGoldClicked);
	}
	if (UpgradeGold)
	{
		UpgradeGold->OnCustomButtonClicked.AddDynamic(this,&UWorkRoomSettingWidget::OnUpgradeCliqued);
	}
	if (UpgradeGem)
	{
		UpgradeGem->OnCustomButtonClicked.AddDynamic(this,&UWorkRoomSettingWidget::OnGemClicked);
	}
	if (Exit)
	{
		Exit->OnCustomButtonClicked.AddDynamic(this,&UWorkRoomSettingWidget::OnExitClicked);
	}
}
