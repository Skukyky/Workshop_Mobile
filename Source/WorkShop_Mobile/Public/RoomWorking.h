#pragma once
 
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomWorking.generated.h"


class APlayerActor;

USTRUCT(BlueprintType)
struct FStatPerLevel
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Stats")
	int MaxNbrWorker = 0;
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Stats")
	float WorkMultiplier = 0.f;
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Stats")
	int RequiredWorkerForNextUpgrade = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Stats")
	int RequiredMoneyForUpgrade = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Stats")
	int RequiredGemForUpgrade = 0;
};
 
UCLASS()
class WORKSHOP_MOBILE_API ARoomWorking : public AActor
{
	GENERATED_BODY()
	
public:	
	ARoomWorking();

	UFUNCTION()
	bool CanUpgradeWithMoney();

	UFUNCTION()
	bool CanUpgradeWithGem();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Stats")
	bool CanUpgrade;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Stats")
	TArray<FStatPerLevel> StatPerLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Stats")
	int LevelRoom;

	UFUNCTION()
	void Upgrade();

	UPROPERTY()
	APlayerActor* PlayerActor;
 
public:	
	virtual void Tick(float DeltaTime) override;
};


