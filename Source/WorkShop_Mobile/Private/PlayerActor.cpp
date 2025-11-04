// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerActor.h"

#include <string>

// Sets default values
APlayerActor::APlayerActor()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComp);
	RootComp->SetMobility(EComponentMobility::Movable);

	
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	camera->SetupAttachment(RootComp);

	AutoPossessPlayer = EAutoReceiveInput::Disabled;

}

void APlayerActor::SetGem(int AddGem)
{
	Gem = Gem + AddGem;
}

int APlayerActor::GetGem() const
{
	return Gem;
}

void APlayerActor::SetMoney(int AddMoney)
{
	Money = Money + AddMoney;
}

int APlayerActor::GetMoney() const
{
	return Money;
}

void APlayerActor::SetPoolResource(int AddPool)
{
	PoolResource = PoolResource + AddPool;
}

int APlayerActor::GetPoolResource() const
{
	return PoolResource;
}

TArray<AWorker*> APlayerActor::GetInventory()
{
	return Workers;
}

void APlayerActor::SetInventory()
{
	//Pas de code pour l'instant mais c le seteur
}

// Called when the game starts or when spawned
void APlayerActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FString Message = FString::Printf(TEXT("Valeur : %d"), GetMoney());
 
	// Afficher le message à l'écran
	GEngine->AddOnScreenDebugMessage(-1,0.0f, FColor::Yellow, Message);
}

// Called to bind functionality to input
void APlayerActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

