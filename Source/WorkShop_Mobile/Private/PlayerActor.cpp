#include "PlayerActor.h"
#include "DelayAction.h"
#include "DrawDebugHelpers.h"
#include "HUDGeneral.h"
#include "Worker.h"

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

APlayerActor::APlayerActor()
{
    PrimaryActorTick.bCanEverTick = true;
    RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    SetRootComponent(RootComp);
    RootComp->SetMobility(EComponentMobility::Movable);

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComp);
    SpringArm->TargetArmLength = 300.f;
    SpringArm->bUsePawnControlRotation = false;

    camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

    AutoPossessPlayer = EAutoReceiveInput::Disabled;
}

void APlayerActor::SetGem(int AddGem)
{
    Gem = Gem + AddGem;
    if (HUDRef)
    {
        HUDRef->UpdateMoneyText(Gem);
    }
}

int APlayerActor::GetGem() const
{
    return Gem;
}

void APlayerActor::SetMoney(int AddMoney)
{
    Money = Money + AddMoney;
    if (HUDRef)
    {
        HUDRef->UpdateMoneyText(Money);
    }
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

void APlayerActor::BeginPlay()
{
    Super::BeginPlay();

    HUDRef = CreateWidget<UHUDGeneral>(GetWorld(), WidgetHUDReference);
    if (HUDRef)
    {
        HUDRef->AddToViewport();
    }
}

void APlayerActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    FString Message = FString::Printf(TEXT("Valeur : %d"), GetMoney());
    GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Yellow, Message);
    ClampCameraWithinBoundary();
}

void APlayerActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerActor::AddWorkerToInventory(FName CharacterID, int32 StarCount)
{
    if (!MyDataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("MyDataTable is null in AddWorkerToInventory"));
        return;
    }

    // Vérifier que le CharacterID existe dans la DataTable
    FCharacterStructure* CharacterRow = MyDataTable->FindRow<FCharacterStructure>(CharacterID, TEXT("AddWorkerToInventory"));
    if (!CharacterRow)
    {
        UE_LOG(LogTemp, Error, TEXT("CharacterID %s not found in DataTable"), *CharacterID.ToString());
        return;
    }

    // Créer la nouvelle entrée de progression
    FCharacterProgress NewProgress;
    NewProgress.CharacterID = CharacterID;
    NewProgress.StarCount = StarCount;
    NewProgress.StatYoutube = CharacterRow->StatYoutube;
    NewProgress.StatTikTok = CharacterRow->StatTikTok;

    // Spawn le worker sur la map
    int32 WorkerIndex = CharactersInventory.Num();
    AWorker* SpawnedWorker = SpawnWorker(CharacterID, WorkerIndex);

    if (SpawnedWorker)
    {
        // Stocker la référence du worker spawné dans la struct
        NewProgress.WorkerSpawnRef = SpawnedWorker;
        
        // Ajouter au tableau d'inventaire
        CharactersInventory.Add(NewProgress);
        
        UE_LOG(LogTemp, Log, TEXT("Worker %s ajouté à l'inventaire et spawné sur la map"), *CharacterID.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Échec du spawn du worker %s"), *CharacterID.ToString());
    }
}

AWorker* APlayerActor::SpawnWorker(FName CharacterID, int32 WorkerIndex)
{
    UWorld* World = GetWorld();
    if (!World || !MyDataTable || !WorkerClass)
    {
        return nullptr;
    }

    FVector SpawnLocation = GetDefaultWorkerSpawnLocation(WorkerIndex);
    FRotator SpawnRotation = FRotator::ZeroRotator;

    AWorker* SpawnedWorker = World->SpawnActor<AWorker>(WorkerClass, SpawnLocation, SpawnRotation);
    
    if (SpawnedWorker)
    {
        SpawnedWorker->MyDataTable = MyDataTable;
        SpawnedWorker->MyIndex = MyDataTable->GetRowNames().Find(CharacterID);
        SpawnedWorker->SetTable();
        
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, SpawnedWorker->GetName());
    }

    return SpawnedWorker;
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
        return;
    }

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

FVector APlayerActor::GetDefaultWorkerSpawnLocation(int32 WorkerIndex) const
{
    // Vous pouvez personnaliser la logique de spawn (par exemple, espacer les workers)
    return DefaultWorkerSpawnLocation + FVector(WorkerIndex * 100.f, 0.f, 0.f);
}