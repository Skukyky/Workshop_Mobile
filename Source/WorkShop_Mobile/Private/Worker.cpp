// Fill out your copyright notice in the Description page of Project Settings.


#include "Worker.h"
#include "AIController.h"
#include "WorkerAIController.h"
#include "CharacterStructure.h"

// Sets default values
AWorker::AWorker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MySceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = MySceneComponent;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComponent->SetupAttachment(MySceneComponent);

	MyCharacterNameText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("My Character Name Text"));
	MyCharacterNameText->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	MyCharacterNameText->SetupAttachment(MySceneComponent);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = AWorkerAIController::StaticClass();

	MyFloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));

}

// Called when the game starts or when spawned
void AWorker::BeginPlay()
{
	Super::BeginPlay();
	SetTable();
}

// Called every frame
void AWorker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWorker::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetTable();
}

void AWorker::SetTable()
{
	if (MyDataTable != nullptr)
	{
		TArray<FName> Rows = MyDataTable->GetRowNames();
		MyIndex = FMath::Clamp(MyIndex, 0, Rows.Num() - 1);
		static const FString ContextString(TEXT("Finding Row in Character Data Table"));
	
		FCharacterStructure* MyRow = MyDataTable->FindRow<FCharacterStructure>(Rows[MyIndex], ContextString);
		if (MyRow != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Loaded row: %s | Mesh: %s | Name: %s"),
		*Rows[MyIndex].ToString(),
		*GetNameSafe(MyRow->CharacterMesh),
		*MyRow->Name);
			SkeletalMeshComponent->SetSkeletalMesh(MyRow->CharacterMesh);
			MyCharacterNameText->SetText(FText::FromString(MyRow->Name));
		}
	}
}

void AWorker::Move()
{
	
}

