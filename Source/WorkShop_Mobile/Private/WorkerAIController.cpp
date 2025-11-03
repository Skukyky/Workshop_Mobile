#include "WorkerAIController.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"

AWorkerAIController::AWorkerAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWorkerAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 🔹 Lancer un mouvement aléatoire toutes les quelques secondes
	GetWorldTimerManager().SetTimer(MoveTimerHandle, this, &AWorkerAIController::MoveToRandomLocation, Speed, true, Delay);
}

void AWorkerAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWorkerAIController::MoveToRandomLocation()
{
	APawn* MyPawn = GetPawn();
	if (!MyPawn) return;

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys) return;

	FNavLocation RandomPoint;

	// 🔹 Trouve un point atteignable dans un rayon autour du Pawn
	bool bFound = NavSys->GetRandomReachablePointInRadius(MyPawn->GetActorLocation(), Radius, RandomPoint);

	if (bFound)
	{
		// 🔹 Déplace l'IA vers ce point
		MoveToLocation(RandomPoint.Location);
	}
}
