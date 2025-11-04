// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomWorking.h"
#include "PlayerActor.h"
#include "Worker.h"

// Sets default values
ARoomWorking::ARoomWorking()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

bool ARoomWorking::CanUpgradeWithMoney()
{
	if (PlayerActor)
	{
		if (StatPerLevel[LevelRoom].RequiredMoneyForUpgrade <= PlayerActor->GetMoney() && StatPerLevel[LevelRoom].RequiredWorkerForNextUpgrade <= 2 && CanUpgrade)
		{
			PlayerActor->SetMoney(-StatPerLevel[LevelRoom].RequiredMoneyForUpgrade);
			Upgrade();
			return true;
		}
	}
	return false;
}

void ARoomWorking::Upgrade()
{
	LevelRoom++;
	WorkMultiplierOnCurrentLevel = StatPerLevel[LevelRoom].WorkMultiplier;
	if (PlayerActor)
	{
		PlayerActor->SetPoolResource(117);
	}
	for (AWorker* Worker : Workers)
	{
		Worker->AddBonusPerRoom();
	}
	int Num = (StatPerLevel[LevelRoom].MaxNbrWorker - 1) - Workers.Num() ;
	for (int i = 0; i<Num - 1; i++)
	{
		Workers.Add(nullptr);
	}
}


bool ARoomWorking::CanUpgradeWithGem()
{
	if (PlayerActor)
	{
		if (StatPerLevel[LevelRoom].RequiredGemForUpgrade <= PlayerActor->GetGem() && StatPerLevel[LevelRoom].RequiredWorkerForNextUpgrade <= 2 && CanUpgrade)
		{
			PlayerActor->SetGem(-StatPerLevel[LevelRoom].RequiredGemForUpgrade);
			Upgrade();
			return true;
		}
	}
	return false;
}

void ARoomWorking::AddMoney(int NewMoney)
{
	int MoneyTemp = CurrentMoneyInStock + NewMoney;
	int MaxMoney = StatPerLevel[LevelRoom].MaxMoneyStorable;
	CurrentMoneyInStock = FMath::Clamp(MoneyTemp, 0, MaxMoney);
 
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,
			FString::Printf(TEXT("MoneyTemp: %d, Clamp: [0, %d], Result: %d"), MoneyTemp, MaxMoney, CurrentMoneyInStock));
	}
	if (CurrentMoneyInStock >= MaxMoney)
	{
		for (AWorker* Worker : Workers)
		{
			if (Worker)
			{
				Worker->StopWorking();
			}
		}
		SendMoneyToPlayer();
	}
}

void ARoomWorking::SendMoneyToPlayer()
{
	if (PlayerActor)
	{
		PlayerActor->SetMoney(CurrentMoneyInStock);
		CurrentMoneyInStock = 0;
		for (AWorker* Worker : Workers)
		{
			if (Worker)
			{
				Worker->StartWorking();
			}
		}
	}
}

void ARoomWorking::AddWorker(int position, AWorker* worker)
{
	Workers[position] = worker;
}

// Called when the game starts or when spawned
void ARoomWorking::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerActor = Cast<APlayerActor>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

// Called every frame
void ARoomWorking::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

