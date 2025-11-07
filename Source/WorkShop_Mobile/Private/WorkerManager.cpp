// Fill out your copyright notice in the Description page of Project Settings.


#include "WorkerManager.h"

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
	
}

// Called every frame
void AWorkerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

