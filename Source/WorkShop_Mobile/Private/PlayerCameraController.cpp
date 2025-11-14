#include "PlayerCameraController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "PlayerActor.h"
#include "RoomWorking.h"
#include "Worker.h"

bool APlayerCameraController::GetHitUnderFingerByChannel(FVector2D ScreenPosition, ECollisionChannel TraceChannel, FHitResult& OutHit)
{
	FVector WorldLocation, WorldDirection;
	if (DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, WorldLocation, WorldDirection))
	{
		FVector Start = WorldLocation;
		FVector End = Start + (WorldDirection * 10000.f);

		FCollisionQueryParams Params;
		Params.bReturnPhysicalMaterial = false;
		Params.AddIgnoredActor(GetPawn());

		return GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, TraceChannel, Params);
	}
	return false;
}

void APlayerCameraController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (InputMappingContext)
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	bEnableTouchEvents = true;

	if (APawn* ControlledPawn = GetPawn())
	{
		OriginPosition = ControlledPawn->GetActorLocation();
	}

	UE_LOG(LogTemp, Warning, TEXT("BeginPlay called — Origin at %s"), *OriginPosition.ToString());
}

void APlayerCameraController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	float X0, Y0, X1, Y1;
	bool bPressed0, bPressed1;
	GetInputTouchState(ETouchIndex::Touch1, X0, Y0, bPressed0);
	GetInputTouchState(ETouchIndex::Touch2, X1, Y1, bPressed1);

	if (bPressed0 && bPressed1)
	{
		FVector2D Touch0(X0, Y0);
		FVector2D Touch1(X1, Y1);
		float Distance = FVector2D::Distance(Touch0, Touch1);

		if (LastTouchDistance == 0.f)
		{
			LastTouchDistance = Distance;
			return;
		}

		float Delta = Distance - LastTouchDistance;
		LastTouchDistance = Distance;
		
		if (FMath::Abs(Delta) > KINDA_SMALL_NUMBER)
		{
			APawn* ControlledPawn = GetPawn();
			if (ControlledPawn)
			{
				APlayerActor* PlayerREF = Cast<APlayerActor>(ControlledPawn);
				if (PlayerREF && PlayerREF->SpringArm)
				{
					// This is equivalent to your scroll zoom:
					float NewLength = FMath::Clamp(
						PlayerREF->SpringArm->TargetArmLength - Delta * ZoomSpeed * PinchZoomSensitivity,
						ZoomMin,
						ZoomMax
					);
					PlayerREF->SpringArm->TargetArmLength = NewLength;

					UE_LOG(LogTemp, Warning, TEXT("PinchZoom Adjusted SpringArm Length: %f"), NewLength);
				}
			}
		}
	}
	else
	{
		LastTouchDistance = 0.f;
	}
}

void APlayerCameraController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (TouchPressedAction)
		{
			EIC->BindAction(TouchPressedAction, ETriggerEvent::Started, this, &APlayerCameraController::OnTouchPressed);
			EIC->BindAction(TouchPressedAction, ETriggerEvent::Completed, this, &APlayerCameraController::OnTouchReleased);
			EIC->BindAction(TouchPressedAction, ETriggerEvent::Canceled, this, &APlayerCameraController::OnTouchReleased);
		}
		if (TouchPositionAction)
		{
			EIC->BindAction(TouchPositionAction, ETriggerEvent::Triggered, this, &APlayerCameraController::OnTouchPosition);
		}
		if (ZoomAction)
		{
			EIC->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &APlayerCameraController::OnZoomTriggered);
		}
	}
}


void APlayerCameraController::OnTouchPressed(const FInputActionValue& Value)
{
	bTouching = true;
	bFirstTouch = true;
	bHasMoved = false;
    
	float X, Y;
	bool bPressed;
	GetInputTouchState(ETouchIndex::Touch1, X, Y, bPressed);
	if (bPressed)
	{
		InitialTouchPosition = FVector2D(X, Y);
		LastTouchPosition = InitialTouchPosition;

		//UE_LOG(LogTemp, Warning, TEXT("TouchPressed at %s"), *InitialTouchPosition.ToString());
	}
}

void APlayerCameraController::OnTouchReleased(const FInputActionValue& Value)
{
	
	if (!bHasMoved)
	{
		OnTouchSelect(FInputActionValue(InitialTouchPosition));
		UE_LOG(LogTemp, Warning, TEXT("InitialTouchPosition at %s"), *InitialTouchPosition.ToString());
	}

	bTouching = false;
}

void APlayerCameraController::OnTouchPosition(const FInputActionValue& Value)
{
	if (!bTouching) return;

	FVector2D CurrentPosition = Value.Get<FVector2D>();

	if (bFirstTouch)
	{
		LastTouchPosition = CurrentPosition;
		bFirstTouch = false;
		return;
	}

	FVector2D TotalDelta = CurrentPosition - InitialTouchPosition;
	//UE_LOG(LogTemp, Warning, TEXT("InitialTouchPosition at %s"), *InitialTouchPosition.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("CurrentPosition at %s"), *CurrentPosition.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("TotalDelta at %s"), *TotalDelta.ToString());
	if (!bHasMoved && TotalDelta.Size() > TouchMoveThreshold)
	{
		bHasMoved = true;
	}

	FVector2D Delta = CurrentPosition - LastTouchPosition;
	LastTouchPosition = CurrentPosition;

	if (bHasMoved)
	{
		if (APawn* ControlledPawn = GetPawn())
		{
			FVector MoveDirection(Delta.Y * PanSpeed, -Delta.X * PanSpeed, 0.f);
			ControlledPawn->AddActorWorldOffset(MoveDirection, true);
		}
	}
}

void APlayerCameraController::OnZoomTriggered(const FInputActionValue& Value)
{
	float ZoomValue = Value.Get<float>();
	if (FMath::Abs(ZoomValue) > KINDA_SMALL_NUMBER)
	{
		APawn* ControlledPawn = GetPawn();
		if (ControlledPawn)
		{
			APlayerActor* PlayerREF = Cast<APlayerActor>(ControlledPawn);
			if (PlayerREF && PlayerREF->SpringArm)
			{
				float NewLength = FMath::Clamp(
					PlayerREF->SpringArm->TargetArmLength - ZoomValue * ZoomSpeed,
					ZoomMin,
					ZoomMax
				);
				PlayerREF->SpringArm->TargetArmLength = NewLength;

				UE_LOG(LogTemp, Warning, TEXT("Adjusted SpringArm Length: %f"), NewLength);
			}
		}
	}
}

void APlayerCameraController::OnTouchSelect(const FInputActionValue& Value)
{
	FVector2D ScreenPosition = Value.Get<FVector2D>();

	FHitResult HitResult;
	if (GetHitUnderFingerByChannel(ScreenPosition, ECC_Visibility, HitResult))
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			UE_LOG(LogTemp, Log, TEXT("Touched Actor: %s"), *HitActor->GetName());
			if (AWorker* Worker = Cast<AWorker>(HitActor))
			{
				int32 Index = Worker->MyIndex;
				if (Worker->MyDataTable)
				{
					TArray<FName> RowNames = Worker->MyDataTable->GetRowNames();
					if (RowNames.IsValidIndex(Index))
					{
						static const FString ContextString(TEXT("DataTable Row Lookup"));
						FCharacterStructure* Row = Worker->MyDataTable->FindRow<FCharacterStructure>(RowNames[Index], ContextString);
						if (Row)
						{
							UE_LOG(LogTemp, Log, TEXT("Touched Worker: %s (Index %d)"), *Row->Name, Index);
						}
					}
				}
			}
			if (ARoomWorking* Room = Cast<ARoomWorking>(HitActor))
			{
				Room->SpawnWidget();
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("No actor hit under finger"));
		}
	}
}
