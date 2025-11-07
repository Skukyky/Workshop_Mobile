// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "CharacterProgress.h"
#include "PlayerCameraController.h"
#include "PlayerActor.generated.h"

class AWorker;

UCLASS()
class WORKSHOP_MOBILE_API APlayerActor : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerActor();

	UFUNCTION()
	void SetGem(int AddGem);

	UFUNCTION()
	int GetGem() const;

	UFUNCTION()
	void SetMoney(int AddMoney);

	UFUNCTION()
	int GetMoney() const;

	UFUNCTION()
	void SetPoolResource(int AddPool);

	UFUNCTION()
	int GetPoolResource() const;

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	int Money;

	UPROPERTY(EditAnywhere)
	USceneComponent* RootComp;

	UPROPERTY()
	int Gem;

	UPROPERTY(EditAnywhere)
	UCameraComponent* camera;
	
	UPROPERTY()
	int PoolResource;
	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//SAVE
	UFUNCTION(BlueprintCallable)
	void SaveInventory();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* SpringArm;
	
	UFUNCTION(BlueprintCallable)
	void LoadInventory();
	void ClampCameraWithinBoundary();
	void OnConstruction(const FTransform& Transform);

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Camera Boundary", meta=(MakeEditWidget=true))
	TArray<FVector> CameraBoundaryPoints;

	UPROPERTY(BlueprintReadWrite)
	TArray<FCharacterProgress> CharactersInventory;

	UFUNCTION()
	void SpawnWorkersFromInventory();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character")
	UDataTable* MyDataTable;

	UFUNCTION()
	FVector GetDefaultWorkerSpawnLocation(int32 WorkerIndex) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DefaultWorkerSpawnLocation = FVector(0, 0, 0);
	
};
