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
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Character)
	UTexture2D* Photo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Character)
	USkeletalMesh* CharacterMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Character)
	float StatYoutube;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Character)
	float StatTikTok;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Character)
	float StatProduction;
};
