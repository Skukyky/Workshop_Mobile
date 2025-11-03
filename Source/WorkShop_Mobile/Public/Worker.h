// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Worker.generated.h"

UCLASS()
class WORKSHOP_MOBILE_API AWorker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere)
	USceneComponent* MySceneComponent;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UDataTable* MyDataTable;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent* MyCharacterNameText;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 MyIndex;

	UFUNCTION(BlueprintCallable)
	void SetTable();

};
