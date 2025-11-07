// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Actor.h"
#include "GachaCharacterShowcase.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class WORKSHOP_MOBILE_API AGachaCharacterShowcase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGachaCharacterShowcase();

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* CharacterMesh = nullptr;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent* Name = nullptr;
	
	UPROPERTY(EditAnywhere)
	USceneCaptureComponent2D* CharacterCapture = nullptr;

	UPROPERTY(EditAnywhere)
	USceneComponent* RootComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	UNiagaraComponent* NiagaraStar;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UNiagaraSystem* NiagaraSystemAsset;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Prend le nom de la ligne dans data table pour mettre à jour le mesh
	void SetCharacterByRowName(FName RowName);

	// Table de données assignée (set via blueprint ou code)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	UDataTable* CharacterDataTable;

};
