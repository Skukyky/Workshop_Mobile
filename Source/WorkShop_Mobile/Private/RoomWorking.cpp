// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomWorking.h"
#include "PlayerActor.h"
#include "WorkRoomSettingWidget.h"
#include "Worker.h"
#include "Blueprint/UserWidget.h"

// Sets default values
ARoomWorking::ARoomWorking()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	SetRootComponent(StaticMeshComponent);

}


void ARoomWorking::SetUp()
{
	Workers.SetNum(StatPerLevel[LevelRoom].MaxNbrWorker);
	for (FWorkerAssigned Worker : Workers)
	{
		if (Worker.Worker != nullptr)
		{
			Worker.Worker->AssignWork(this);
		}
	}
}

void ARoomWorking::Upgrade()
{
	LevelRoom++;
	WorkMultiplierOnCurrentLevel = StatPerLevel[LevelRoom].WorkMultiplier;
	if (PlayerActor)
	{
		PlayerActor->SetPoolResource(117);
	}
	for (FWorkerAssigned Worker : Workers)
	{
		if (Worker.Worker != nullptr)
		{
			Worker.Worker->AddBonusPerRoom();
		}
		
	}
	Workers.SetNum(StatPerLevel[LevelRoom].MaxNbrWorker);
	SpawnWidget();
}


bool ARoomWorking::CanUpgradeWithGem()
{
	if (PlayerActor)
	{
		if (StatPerLevel[LevelRoom].RequiredGemForUpgrade <= PlayerActor->GetGem() && StatPerLevel[LevelRoom].RequiredFollowerForNextUpgrade <= 2 && CanUpgrade)
		{
			PlayerActor->SetGem(-StatPerLevel[LevelRoom].RequiredGemForUpgrade);
			Upgrade();
			return true;
		}
	}
	return false;
}

bool ARoomWorking::CanUpgradeWithMoney()
{
	if (PlayerActor)
	{
		if (StatPerLevel[LevelRoom].RequiredMoneyForUpgrade <= PlayerActor->GetMoney() && StatPerLevel[LevelRoom].RequiredFollowerForNextUpgrade <= 2 && CanUpgrade)
		{
			PlayerActor->SetMoney(-StatPerLevel[LevelRoom].RequiredMoneyForUpgrade);
			Upgrade();
			return true;
		}
		else
		{
			CanUpgradeWithGem();
		}
	}
	return false;
}

void ARoomWorking::AddMoney(float NewMoney)
{
	float MoneyTemp = CurrentMoneyInStock + NewMoney;
	float MaxMoney = StatPerLevel[LevelRoom].MaxMoneyStorable;
	CurrentMoneyInStock = FMath::Clamp(MoneyTemp, 0, MaxMoney);
	
	if (CurrentMoneyInStock >= MaxMoney)
	{
		for (FWorkerAssigned Worker : Workers)
		{
			if (Worker.Worker != nullptr)
			{
				Worker.Worker->StopWorking();
			}
		}
	}
}



void ARoomWorking::SendMoneyToPlayer()
{
	if (PlayerActor)
	{
		PlayerActor->SetMoney(CurrentMoneyInStock);
		CurrentMoneyInStock = 0;
		for (FWorkerAssigned Worker : Workers)
		{
			if (Worker.Worker != nullptr)
			{
				Worker.Worker->StartWorking();
			}
		}
	}
}

void ARoomWorking::AddWorker(int position, AWorker* worker)
{
	//FIX
	if (Workers.Num() - 1 >= position)
	{
		if (Workers[position].Worker) Workers[position].Worker->StopWorking();
		Workers[position] = {worker, FVector2D::ZeroVector};
		worker->AssignWork(this);
		SpawnWidget();
	}

}

void ARoomWorking::SpawnWidget()
{
	if (Widget)
	{
		if (RoomSettingWidget)
		{
			RoomSettingWidget->Refresh();
			return;
		}
		RoomSettingWidget = CreateWidget<UWorkRoomSettingWidget>(GetWorld(), Widget);
		if (RoomSettingWidget)
		{
			RoomSettingWidget->RoomWorking = this;
			RoomSettingWidget->AddToViewport();
		}
		
	}
	
}

// Called when the game starts or when spawned
void ARoomWorking::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerActor = Cast<APlayerActor>(GetWorld()->GetFirstPlayerController()->GetPawn());
	SetUp();

}

// Called every frame
void ARoomWorking::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (FWorkerAssigned Worker : Workers)
	{
		if (Worker.Worker != nullptr)
		{
			FString Message = FString::Format(TEXT("Working Time: {0}"), { Worker.Worker->GetName() });
			GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, Message);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, TEXT("Nullptr"));
		}
	}
	
}

