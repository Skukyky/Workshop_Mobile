// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomWorking.h"
#include "PlayerActor.h"

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
			Upgrade();
			return true;
		}
	}
	return false;
}

void ARoomWorking::Upgrade()
{
	LevelRoom++;

}


bool ARoomWorking::CanUpgradeWithGem()
{
	if (PlayerActor)
	{
		if (StatPerLevel[LevelRoom].RequiredGemForUpgrade <= PlayerActor->GetGem() && StatPerLevel[LevelRoom].RequiredWorkerForNextUpgrade <= 2 && CanUpgrade)
		{
			Upgrade();
			return true;
		}
	}
	return false;
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

