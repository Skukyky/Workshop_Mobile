// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStructure.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Worker.generated.h"

class APlayerActor;
class ARoomWorking;

UCLASS()
class WORKSHOP_MOBILE_API AWorker : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorker();

	UPROPERTY()
	float MoneyPerWorkWithBonus;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void Working();

	FTimerHandle TimerHandle;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	ARoomWorking* RoomWorking;

	UPROPERTY()
	float WorkingTimeBase;

	UPROPERTY()
	float WorkingTimeWithBonus;

	UPROPERTY()
	float MoneyPerWorkBase;
	
	
	FCharacterStructure* MyRow;

	UPROPERTY()
	APlayerActor* PlayerActor;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY()
	float MoneyPerWork;
	
	UPROPERTY(EditAnywhere)
	USceneComponent* MySceneComponent;

	UFUNCTION()
	FCharacterStructure GetMyCharacterStructure() const;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UDataTable* MyDataTable;

	UPROPERTY()
	UFloatingPawnMovement* MyFloatingPawnMovement;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent* MyCharacterNameText;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 MyIndex;

	UFUNCTION(BlueprintCallable)
	void SetTable();

	UFUNCTION()
	void StartWorking();

	UFUNCTION()
	void StopWorking();

	UFUNCTION()
	void AssignWork(ARoomWorking* Working);

	UFUNCTION()
	void UnassignWork();

	UFUNCTION()
	void AddBonusPerRoom();

	UFUNCTION()
	void AddBonusPerStars();

};
