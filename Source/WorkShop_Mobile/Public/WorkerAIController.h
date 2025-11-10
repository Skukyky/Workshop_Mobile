#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "WorkerManager.h"
#include "WorkerAIController.generated.h"

UCLASS()
class WORKSHOP_MOBILE_API AWorkerAIController : public AAIController
{
	GENERATED_BODY()

public:
	AWorkerAIController();

	bool tasked = false;
	FVector WaitPosition;
	FVector WorkPosition;

	int32 CurrentWaitIndex = -1;
	int32 CurrentWorkIndex = -1;

	UPROPERTY()
	APawn* MyPawn;

	UPROPERTY(EditAnywhere)
	AWorkerManager* Manager;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;

private:
	void MoveToLocation();
	void AssignWaitPosition();
	void AssignWorkPosition();
	void ReleasePositions();
	void SwitchToWorkPosition();
	void SwitchToWaitPosition();

	FTimerHandle MoveTimerHandle;

	float Speed = 3.0f;
	float Delay = 3.0f;
	float Radius = 1000.0f;
	
};
