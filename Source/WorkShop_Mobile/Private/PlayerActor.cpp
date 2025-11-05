// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerActor.h"

#include <string>

#include "GachaSaveGame.h"
#include "Kismet/GameplayStatics.h"

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
	LoadInventory();

	if (GEngine)
	{
		if (CharactersInventory.Num() == 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Inventaire vide au début du jeu."));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Inventaire personnages chargés :"));
			for (const FCharacterProgress& Elem : CharactersInventory)
			{
				FString Msg = FString::Printf(TEXT("- %s : Etoiles %d"), *Elem.CharacterID.ToString(), Elem.StarCount);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Msg);
			}
		}
	}
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

void APlayerActor::SaveInventory()
{
	UGachaSaveGame* SaveGameInstance = nullptr;

	if (UGameplayStatics::DoesSaveGameExist(TEXT("GachaSaveSlot"), 0))
	{
		SaveGameInstance = Cast<UGachaSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("GachaSaveSlot"), 0));
	}

	if (!SaveGameInstance)
	{
		SaveGameInstance = Cast<UGachaSaveGame>(UGameplayStatics::CreateSaveGameObject(UGachaSaveGame::StaticClass()));
	}

	if (SaveGameInstance)
	{
		SaveGameInstance->SavedCharactersArray = CharactersInventory; // utiliser le tableau dans la classe de save

		bool bSuccess = UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("GachaSaveSlot"), 0);
		if (bSuccess)
		{
			UE_LOG(LogTemp, Log, TEXT("Inventaire personnages sauvegardé."));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Erreur lors de la sauvegarde de l'inventaire personnages."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Impossible de créer ou charger la sauvegarde."));
	}
}

void APlayerActor::LoadInventory()
{
	if (!UGameplayStatics::DoesSaveGameExist(TEXT("GachaSaveSlot"), 0))
		return;

	UGachaSaveGame* LoadedGame = Cast<UGachaSaveGame>(
	   UGameplayStatics::LoadGameFromSlot(TEXT("GachaSaveSlot"), 0)
	);

	if (LoadedGame)
	{
		CharactersInventory = LoadedGame->SavedCharactersArray;
		UE_LOG(LogTemp, Log, TEXT("Inventaire personnages chargé."));
	}
}
