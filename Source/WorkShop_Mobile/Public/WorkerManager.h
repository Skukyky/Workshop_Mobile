// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Worker.h"
#include "GameFramework/Actor.h"
#include "WorkerManager.generated.h"

UCLASS()
class WORKSHOP_MOBILE_API AWorkerManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorkerManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ReleaseWaitPosition(int32 Index);
	void ReleaseWorkPosition(int32 Index);
	

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Worker Post", meta=(MakeEditWidget=true))
	TArray<FVector> WaitPosition;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Worker Post", meta=(MakeEditWidget=true))
	TArray<FVector> WorkPosition;

	TArray<bool> bWaitPositionOccupied;
	TArray<bool> bWorkPositionOccupied;

	int32 AcquireWorkPosition();
	int32 AcquireWaitPosition();
	
	
};
