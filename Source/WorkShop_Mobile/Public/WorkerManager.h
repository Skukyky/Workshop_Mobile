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

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Worker Post", meta=(MakeEditWidget=true))
	TArray<FVector2D> WorkPosition;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TArray<AWorker*> WorkerTasked;

};
