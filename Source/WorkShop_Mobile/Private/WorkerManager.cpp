// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkerManager.h"


int32 AWorkerManager::AcquireWaitPosition()
{
	for (int32 i = 0; i < bWaitPositionOccupied.Num(); i++)
	{
		if (!bWaitPositionOccupied[i])
		{
			bWaitPositionOccupied[i] = true;
			return i;
		}
	}
	return -1;
}

// Retourne l'indice d'une position de travail libre et la marque occupée, ou -1 si aucune dispo
int32 AWorkerManager::AcquireWorkPosition()
{
	for (int32 i = 0; i < bWorkPositionOccupied.Num(); i++)
	{
		if (!bWorkPositionOccupied[i])
		{
			bWorkPositionOccupied[i] = true;
			return i;
		}
	}
	return -1;
}

// Sets default values
AWorkerManager::AWorkerManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWorkerManager::BeginPlay()
{
	Super::BeginPlay();

	bWaitPositionOccupied.Init(false, WaitPosition.Num());
	bWorkPositionOccupied.Init(false, WorkPosition.Num());
}

// Called every frame
void AWorkerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWorkerManager::ReleaseWaitPosition(int32 Index)
{
	if (bWaitPositionOccupied.IsValidIndex(Index))
	{
		bWaitPositionOccupied[Index] = false;
	}
}

// Libère une position de travail donnée
void AWorkerManager::ReleaseWorkPosition(int32 Index)
{
	if (bWorkPositionOccupied.IsValidIndex(Index))
	{
		bWorkPositionOccupied[Index] = false;
	}
}
