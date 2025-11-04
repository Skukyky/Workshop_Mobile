#pragma once
 
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomWorking.generated.h"


class APlayerActor;
class AWorker;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Stats")
	int MaxMoneyStorable = 1000;
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

	// UFUNCTION()
	// void AddMoney(int NewMoney);

	UFUNCTION()
	void SendMoneyToPlayer();
	
	UPROPERTY()
	float WorkMultiplierOnCurrentLevel = 0.f;

	UPROPERTY()
	TArray<AWorker*> Workers;

	
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	bool CanUpgrade = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Stats")
	TArray<FStatPerLevel> StatPerLevel;

	UPROPERTY()
	int LevelRoom = 0;

	UFUNCTION()
	void Upgrade();

	UPROPERTY()
	APlayerActor* PlayerActor = nullptr;

	UPROPERTY()
	int CurrentMoneyInStock = 0;



public:	
	virtual void Tick(float DeltaTime) override;
};


