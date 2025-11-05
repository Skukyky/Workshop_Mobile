#pragma once

#include "CoreMinimal.h"
#include "CharacterProgress.generated.h"

class AWorker;

USTRUCT(BlueprintType)
struct FCharacterProgress
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SaveData")
	FName CharacterID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SaveData")
	int32 StarCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SaveData")
	float StatYoutube = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SaveData")
	float StatTikTok = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SaveData")
	AWorker* WorkerSpawnRef = nullptr;

	// Constructeur par défaut
	FCharacterProgress()
		: CharacterID(NAME_None), StarCount(0), StatYoutube(0.f), StatTikTok(0.f)
	{}
};
