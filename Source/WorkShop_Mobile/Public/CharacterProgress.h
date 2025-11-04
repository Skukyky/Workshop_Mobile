#pragma once
#include "CharacterProgress.generated.h"
USTRUCT(BlueprintType)
struct FCharacterProgress
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FName CharacterRowName;

	UPROPERTY(BlueprintReadWrite)
	int StarCount = 0;

	UPROPERTY(BlueprintReadWrite)
	float StatYoutube = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float StatTikTok = 0.f;
	
};
