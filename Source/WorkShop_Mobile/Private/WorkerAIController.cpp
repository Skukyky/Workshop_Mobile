#include "WorkerAIController.h"
#include "GameFramework/Actor.h"
#include "Worker.h"
#include "EngineUtils.h"
#include "RoomWorking.h"

AWorkerAIController::AWorkerAIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWorkerAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!Manager)
	{
		// Trouver le WorkerManager dans la scène (par exemple le premier trouvé)
		for (TActorIterator<AWorkerManager> It(GetWorld()); It; ++It)
		{
			Manager = *It;
			break;
		}
	}

	MyPawn = Cast<AWorker>(GetPawn());
	
	// Assigner une place d’attente au spawn
	AssignWaitPosition();
}

void AWorkerAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (tasked && CurrentWorkIndex == -1)
	{
		SwitchToWorkPosition();
	}
	else if (!tasked && CurrentWaitIndex == -1)
	{
		SwitchToWaitPosition();
	}
}

void AWorkerAIController::MoveToLocation()
{
	
}

void AWorkerAIController::AssignWaitPosition()
{
    if (Manager)
    {
        int32 Index = Manager->AcquireWaitPosition();
        if (Index != -1)
        {
            CurrentWaitIndex = Index;
        	WaitPosition = Manager->GetActorTransform().TransformPosition(Manager->WaitPosition[Index]);

            // Visual debug
            DrawDebugSphere(GetWorld(), WaitPosition, 30.f, 12, FColor::Green, false, 5.f);

            // Déplacement AI controller
            MyPawn->SetActorLocation(WaitPosition, false);
        }
    }
}

void AWorkerAIController::AssignWorkPosition()
{
	if (Manager && tasked && MyPawn)
	{
		/*
		int32 Index = Manager->AcquireWorkPosition();
		if (Index != -1)
		{
			CurrentWorkIndex = Index;
			WorkPosition = Manager->GetActorTransform().TransformPosition(Manager->WorkPosition[Index]);
			// Teleport or move to WorkPosition
			MyPawn->SetActorLocation(WorkPosition);
		}*/
		int Indexing = 0;
		CurrentWorkIndex = Indexing;
		for (int i = 0; i<MyPawn->RoomWorking->Workers.Num(); i++)
		{
			if (MyPawn->RoomWorking->Workers[i].Worker == MyPawn)
			{
				Indexing = i;
				break;
			}
			else
			{
				Indexing = -1;
			}
		}
		CurrentWorkIndex = Indexing;
		if (Indexing != -1)
		{
			FVector Position = MyPawn->RoomWorking->StatPerLevel[MyPawn->RoomWorking->LevelRoom].NewGamingPC[Indexing]->GetActorLocation();
			FRotator Rotation = MyPawn->RoomWorking->StatPerLevel[MyPawn->RoomWorking->LevelRoom].NewGamingPC[Indexing]->GetActorRotation();
			MyPawn->SetActorLocation(FVector(Position.X, Position.Y, Position.Z + 50));
			MyPawn->SetActorRotation(Rotation);
		}
	}
}

void AWorkerAIController::ReleasePositions()
{
	if (Manager)
	{
		if (CurrentWaitIndex != -1)
		{
			Manager->ReleaseWaitPosition(CurrentWaitIndex);
			CurrentWaitIndex = -1;
		}
		if (CurrentWorkIndex != -1)
		{
			Manager->ReleaseWorkPosition(CurrentWorkIndex);
			CurrentWorkIndex = -1;
		}
	}
}

void AWorkerAIController::SwitchToWorkPosition()
{
	if (Manager && tasked)
	{
		// Libérer la position d’attente si on en a une
		if (CurrentWaitIndex != -1)
		{
			Manager->ReleaseWaitPosition(CurrentWaitIndex);
			CurrentWaitIndex = -1;
		}

		// Acquérir une position de travail
		int32 Index = Manager->AcquireWorkPosition();
		if (Index != -1)
		{
			/*
			CurrentWorkIndex = Index;
			WorkPosition = Manager->GetActorTransform().TransformPosition(Manager->WorkPosition[Index]);
			MyPawn->SetActorLocation(WorkPosition);*/

			AssignWorkPosition();
		}
	}
}

void AWorkerAIController::SwitchToWaitPosition()
{
	if (Manager)
	{
		// Libérer la position de travail si on en a une
		if (CurrentWorkIndex != -1)
		{
			Manager->ReleaseWorkPosition(CurrentWorkIndex);
			CurrentWorkIndex = -1;
			tasked = false; // Optionnel : reset tâche ici
		}

		// Acquérir une position d’attente
		int32 Index = Manager->AcquireWaitPosition();
		if (Index != -1)
		{
			CurrentWaitIndex = Index;
			WaitPosition = Manager->GetActorTransform().TransformPosition(Manager->WaitPosition[Index]);
			MyPawn->SetActorLocation(WaitPosition);
		}
	}
}