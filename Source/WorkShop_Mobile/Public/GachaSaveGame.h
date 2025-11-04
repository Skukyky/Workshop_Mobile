// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CharacterProgress.h"
#include "GachaSaveGame.generated.h"

struct FCharacterProgress;
/**
 * 
 */
UCLASS()
class WORKSHOP_MOBILE_API UGachaSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	TMap<FName, FCharacterProgress> SavedCharacters;
};
