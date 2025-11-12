#pragma once
 
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "tuple"
#include "RoomWorking.generated.h"


class UWorkRoomSettingWidget;
class APlayerActor;
class AWorker;

USTRUCT(BlueprintType)
struct FStatPerLevel
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Stats")
	int MaxNbrWorker = 0;
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Stats")
	float WorkMultiplier = 0.5f;
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Stats")
	int RequiredFollowerForNextUpgrade = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Stats")
	int RequiredMoneyForUpgrade = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Stats")
	int RequiredGemForUpgrade = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Stats")
	int MaxMoneyStorable = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Stats")
	TArray<AActor*> NewGamingPC;
	
};

USTRUCT(BlueprintType)
struct FWorkerAssigned
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Stats")
	AWorker* Worker = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Stats")
	FVector2D Location = FVector2D::ZeroVector;
	
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

	UFUNCTION()
	void AddMoney(float NewMoney);
	

	UFUNCTION()
	void SendMoneyToPlayer();
	
	UPROPERTY()
	float WorkMultiplierOnCurrentLevel = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Stats")
	TArray<FWorkerAssigned> Workers;

	UFUNCTION()
	void AddWorker(int position, AWorker* worker);

	UFUNCTION()
	void SpawnWidget();

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Room Stats")
	bool IsYoutubeRoom;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	bool CanUpgrade = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Stats")
	TArray<FStatPerLevel> StatPerLevel;

	UPROPERTY()
	int LevelRoom = -1;

	UFUNCTION()
	void SetUp();

	UFUNCTION()
	void Upgrade();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMeshComponent = nullptr;
	
	UPROPERTY()
	APlayerActor* PlayerActor = nullptr;

	UPROPERTY()
	float CurrentMoneyInStock = 0;

	UPROPERTY()
	float CurrentFollowerInStock = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UWorkRoomSettingWidget> Widget;
	
	UPROPERTY()
	UWorkRoomSettingWidget* RoomSettingWidget = nullptr;

public:	
	virtual void Tick(float DeltaTime) override;
};


