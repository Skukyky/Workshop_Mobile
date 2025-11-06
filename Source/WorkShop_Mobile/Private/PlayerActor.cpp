// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerActor.h"
#include "DrawDebugHelpers.h"
#include <string>
#include "GachaSaveGame.h"
#include "Kismet/GameplayStatics.h"

bool IsPointInPolygon(const FVector2D& Point, const TArray<FVector2D>& Polygon)
{
	int32 Intersections = 0;
	for (int32 i = 0, j = Polygon.Num() - 1; i < Polygon.Num(); j = i++)
	{
		const FVector2D& Pi = Polygon[i];
		const FVector2D& Pj = Polygon[j];
		bool cond1 = (Pi.Y > Point.Y) != (Pj.Y > Point.Y);
		float slope = (Pj.X - Pi.X) * (Point.Y - Pi.Y) / (Pj.Y - Pi.Y) + Pi.X;
		if (cond1 && Point.X < slope)
		{
			Intersections++;
		}
	}
	return (Intersections % 2) == 1;
}

static TArray<FVector2D> ConvertTo2D(const TArray<FVector>& Vec3Array)
{
	TArray<FVector2D> Vec2Array;
	Vec2Array.Reserve(Vec3Array.Num());
	for (const FVector& Vec : Vec3Array)
	{
		Vec2Array.Add(FVector2D(Vec.X, Vec.Y));
	}
	return Vec2Array;
}

// Sets default values
APlayerActor::APlayerActor()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComp);
	RootComp->SetMobility(EComponentMobility::Movable);

	// Créer et attacher le SpringArm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComp);
	SpringArm->TargetArmLength = 300.f; // Longueur initiale modifiable en BP
	SpringArm->bUsePawnControlRotation = false; // si tu veux contrôler la rotation du springarm via controller

	// Attacher la caméra au bout du SpringArm
	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

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

	ClampCameraWithinBoundary();
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

void APlayerActor::ClampCameraWithinBoundary()
{
	if (CameraBoundaryPoints.Num() < 3)
	{
		UE_LOG(LogTemp, Log, TEXT("Pas assez de points pour définir une zone"));
		return;
	}

	FVector CamLocation = camera->GetComponentLocation();
	FVector2D FlatLocation(CamLocation.X, CamLocation.Y);

	TArray<FVector2D> Polygon2D = ConvertTo2D(CameraBoundaryPoints);

	if (IsPointInPolygon(FlatLocation, Polygon2D))
	{
		// Position valide
		return;
	}

	// Sinon, clamp sur bbox polygonale 2D
	float MinX = Polygon2D[0].X;
	float MaxX = MinX;
	float MinY = Polygon2D[0].Y;
	float MaxY = MinY;
	for (const FVector2D& Pt : Polygon2D)
	{
		MinX = FMath::Min(MinX, Pt.X);
		MaxX = FMath::Max(MaxX, Pt.X);
		MinY = FMath::Min(MinY, Pt.Y);
		MaxY = FMath::Max(MaxY, Pt.Y);
	}
	float ClampedX = FMath::Clamp(CamLocation.X, MinX, MaxX);
	float ClampedY = FMath::Clamp(CamLocation.Y, MinY, MaxY);

	FVector NewLocation(ClampedX, ClampedY, CamLocation.Z);
	camera->SetWorldLocation(NewLocation);
}

void APlayerActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (CameraBoundaryPoints.Num() < 2)
		return;

	UWorld* World = GetWorld();
	if (!World) return;

	for (int32 i = 0; i < CameraBoundaryPoints.Num(); ++i)
	{
		int32 NextIndex = (i + 1) % CameraBoundaryPoints.Num();

		// Transformation locale → mondiale (si les points sont relatifs au RootComp)
		FVector WorldPointA = RootComp ? RootComp->GetComponentTransform().TransformPosition(CameraBoundaryPoints[i])
									  : CameraBoundaryPoints[i];
		FVector WorldPointB = RootComp ? RootComp->GetComponentTransform().TransformPosition(CameraBoundaryPoints[NextIndex])
									  : CameraBoundaryPoints[NextIndex];

		DrawDebugLine(
			World,
			WorldPointA,
			WorldPointB,
			FColor::Green,
			false,
			-1.f,
			0,
			5.f
		);
	}
}