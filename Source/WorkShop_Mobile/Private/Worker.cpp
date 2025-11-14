// Fill out your copyright notice in the Description page of Project Settings.


#include "Worker.h"
#include "AIController.h"
#include "WorkerAIController.h"
#include "CharacterStructure.h"
#include "PlayerActor.h"
#include "RoomWorking.h"

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
	PlayerActor = Cast<APlayerActor>(GetWorld()->GetFirstPlayerController()->GetPawn());
	ControllerREF = Cast<AWorkerAIController>(GetController());
}

void AWorker::Working()
{
	if (RoomWorking)
	{
		RoomWorking->AddMoney(MoneyPerWorkWithBonus);
	}
}

void AWorker::StartWorking()
{
	if (!TimerHandle.IsValid())
	{
		
		GetWorldTimerManager().SetTimer(TimerHandle,this,&AWorker::Working,WorkingTimeBase,true);
		ControllerREF->tasked = true;
	}
}

void AWorker::StopWorking()
{
	if (TimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		ControllerREF->tasked = false;
		
	}
}

void AWorker::AssignWork(ARoomWorking* Working)
{
	RoomWorking = Working;
	if (RoomWorking && PlayerActor)
	{
		PlayerActor->Workers.AddUnique(this);
		AddBonusPerStars();
	}
}

void AWorker::UnassignWork()
{
	if (PlayerActor)
	{
		int remove = PlayerActor->Workers.Find(this);
		if (remove>-1){PlayerActor->Workers.RemoveAt(remove);}
		StopWorking();
		RoomWorking = nullptr;
	}
}

void AWorker::AddBonusPerRoom()
{
	MoneyPerWorkWithBonus  = MoneyPerWork;
	if (RoomWorking)
	{
		int bonus = RoomWorking->IsYoutubeRoom ? MyRow->StatYoutube : MyRow->StatTikTok;
		MoneyPerWorkWithBonus = MoneyPerWork * RoomWorking->WorkMultiplierOnCurrentLevel;
		MoneyPerWorkWithBonus = MoneyPerWorkWithBonus * bonus;
		StartWorking();
	}
}

void AWorker::AddBonusPerStars()
{
	MoneyPerWork = MoneyPerWorkBase * MyRow->Star + 1;
	AddBonusPerRoom();
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

FCharacterStructure AWorker::GetMyCharacterStructure() const
{
	if (MyRow != nullptr)
	{
		return *MyRow;
	}
	
	UE_LOG(LogTemp, Error, TEXT("Tentative d'accès à MyRow alors qu'il est nullptr dans AWorker!"));
	return FCharacterStructure(); 
}

void AWorker::SetTable()
{
	if (MyDataTable != nullptr)
	{
		TArray<FName> Rows = MyDataTable->GetRowNames();
		MyIndex = FMath::Clamp(MyIndex, 0, Rows.Num() - 1);
		static const FString ContextString(TEXT("Finding Row in Character Data Table"));
	
		MyRow = MyDataTable->FindRow<FCharacterStructure>(Rows[MyIndex], ContextString);
		if (MyRow != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Loaded row: %s | Mesh: %s | Name: %s"),
		*Rows[MyIndex].ToString(),
		*GetNameSafe(MyRow->CharacterMesh),
		*MyRow->Name);
			SkeletalMeshComponent->SetSkeletalMesh(MyRow->CharacterMesh);
			MyCharacterNameText->SetText(FText::FromString(MyRow->Name));
			WorkingTimeBase = MyRow->StatProductionSpeed;
			MoneyPerWorkBase = MyRow->StatProductionMoney;
			AddBonusPerStars();
		}
		
	}
}


