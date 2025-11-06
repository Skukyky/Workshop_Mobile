// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterStructure.generated.h"

// -------------------- ENUMS --------------------

UENUM(BlueprintType)
enum class ECharacterRarity : uint8
{
	Commun      UMETA(DisplayName = "Commun"),
	Rare        UMETA(DisplayName = "Rare"),
	Epique      UMETA(DisplayName = "Epique"),
	Legendary   UMETA(DisplayName = "Legendary"),
	Secret      UMETA(DisplayName = "Secret")
};

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	Youtube     UMETA(DisplayName = "Youtube"),
	TikTok      UMETA(DisplayName = "TikTok")
};

// -------------------- STRUCT --------------------

USTRUCT(BlueprintType)
struct WORKSHOP_MOBILE_API FCharacterStructure : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	FString Name = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	UTexture2D* Photo = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	USkeletalMesh* CharacterMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	int Star = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	int StarMax = 5;
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	float StatYoutube = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	float StatTikTok = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	float StatProductionMoney = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	ECharacterRarity Rarity = ECharacterRarity::Commun;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	ECharacterType Type = ECharacterType::Youtube;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character")
	float StatProductionSpeed = 1.0f;
};
