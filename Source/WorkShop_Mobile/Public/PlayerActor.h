// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
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

	UFUNCTION()
	TArray<AWorker*> GetInventory();

	UFUNCTION()
	void SetInventory();

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

	TArray<AWorker*> Workers;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//SAVE
	UFUNCTION(BlueprintCallable)
	void SaveInventory();

	UFUNCTION(BlueprintCallable)
	void LoadInventory();

	UPROPERTY(BlueprintReadWrite)
	TArray<FCharacterProgress> CharactersInventory;

	

};
