#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CharacterProgress.h"
#include "GachaSaveGame.generated.h"

UCLASS()
class WORKSHOP_MOBILE_API UGachaSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	// Tableau supportant les doublons d'items
	UPROPERTY(BlueprintReadWrite, Category="SaveData")
	TArray<FCharacterProgress> SavedCharactersArray;
};