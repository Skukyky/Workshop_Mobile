#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/UserWidget.h"
#include "HUDGeneral.h"
#include "Components/AudioComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CharacterProgress.h"
#include "PlayerCameraController.h"
#include "PlayerActor.generated.h"

class UTutorialWidget;
class AWorker;

UCLASS()
class WORKSHOP_MOBILE_API APlayerActor : public APawn
{
    GENERATED_BODY()

public:
    APlayerActor();
    
    UFUNCTION()
    void SetGem(int AddGem);
    
    UFUNCTION()
    int GetGem() const {return Gem;}
    
    UFUNCTION()
    void SetMoney(int AddMoney);
    
    UFUNCTION()
    int GetMoney() const {return Money;}
    
    UFUNCTION()
    void SetPoolResource(int AddPool);
    
    UFUNCTION()
    int GetPoolResource() const {return PoolResource;}

    UFUNCTION()
    void SetFollower(int Addfollower);

    UFUNCTION()
    int GetFollower() const {return Follower;}

    // Nouvelle fonction pour ajouter un worker et le spawn
    UFUNCTION(BlueprintCallable)
    void AddWorkerToInventory(FName CharacterID, int32 StarCount = 1);

    // Récupérer le tableau des workers pour l'inventaire
    UFUNCTION(BlueprintCallable)
    TArray<FCharacterProgress>& GetWorkersInventory() { return CharactersInventory; }

    UPROPERTY()
    UUserWidget* Widget;

    UPROPERTY()
    TArray<AWorker*> Workers;

protected:
    virtual void BeginPlay() override;

    UPROPERTY()
    int Money = 100000;

    UPROPERTY(EditAnywhere)
    USceneComponent* RootComp;

    UPROPERTY()
    int Gem = 100;

    UPROPERTY(EditAnywhere)
    UCameraComponent* camera;

    UPROPERTY()
    int PoolResource;

    UPROPERTY()
    int Follower = 0;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    USpringArmComponent* SpringArm;

    void ClampCameraWithinBoundary();
    void OnConstruction(const FTransform& Transform);

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Camera Boundary", meta=(MakeEditWidget=true))
    TArray<FVector> CameraBoundaryPoints;

    // Tableau principal des workers (remplace la save)
    UPROPERTY(BlueprintReadWrite)
    TArray<FCharacterProgress> CharactersInventory;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UHUDGeneral> WidgetHUDReference;

    UPROPERTY()
    UHUDGeneral* HUDRef;

    UPROPERTY(EditAnywhere)
    TSubclassOf<UTutorialWidget> WidgetTutoReference;

    UPROPERTY()
    UTutorialWidget* TutoRef;

    

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character")
    UDataTable* MyDataTable;

    UFUNCTION()
    FVector GetDefaultWorkerSpawnLocation(int32 WorkerIndex) const;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector DefaultWorkerSpawnLocation = FVector(0, 0, 0);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character")
    TSubclassOf<AWorker> WorkerClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
    UAudioComponent* MainMusicAudioComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    USoundCue* MainMusic;

    void ChangeVolumeMusic(float Volume);
private:
    // Fonction interne pour spawn un worker
    AWorker* SpawnWorker(FName CharacterID, int32 WorkerIndex);
};