// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkRoomSettingWidget.h"
#include "BTNCustomWidget.h"
#include "RoomWorking.h"

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
		UE_LOG(LogTemp, Display, TEXT("Work Room Setting"));
		for (int i = 0; i<=RoomWorking->Workers.Num()-1; i++)
		{
			UBTNCustomWidget* NewCustomButton = CreateWidget<UBTNCustomWidget>(this, Button);
			int Column = (i+1)/2 - 1;
			int Row = i%2 - 1;
			GridPanel->AddChildToUniformGrid(NewCustomButton,Row,Column);
		}
	}
}
