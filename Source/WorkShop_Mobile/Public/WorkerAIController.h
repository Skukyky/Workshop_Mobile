#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "WorkerAIController.generated.h"

UCLASS()
class WORKSHOP_MOBILE_API AWorkerAIController : public AAIController
{
	GENERATED_BODY()

public:
	AWorkerAIController();

	bool tasked = false;
	FVector2d WorkPosition;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;

private:
	void MoveToRandomLocation();
	void StartMovement(APawn* InPawn);

	FTimerHandle MoveTimerHandle;

	float Speed = 3.0f;
	float Delay = 3.0f;
	float Radius = 1000.0f;
	
};
