// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StructUtils/UserDefinedStruct.h"
#include "CharacterStructure.generated.h"

/**
 * 
 */
USTRUCT()
struct  WORKSHOP_MOBILE_API FCharacterStructure : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Character)
	FString Name = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Character)
	UTexture2D* Photo = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Character)
	USkeletalMesh* CharacterMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Character)
	float DropRate = 0.0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Character)
	float StatYoutube = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Character)
	float StatTikTok = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Character)
	float StatProduction = 0.0;
};
