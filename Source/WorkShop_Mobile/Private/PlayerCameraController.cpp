#include "PlayerCameraController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

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

	// Enable input features
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
			if (APawn* ControlledPawn = GetPawn())
			{
				FVector Move = ControlledPawn->GetActorForwardVector() * Delta * (ZoomSpeed / 100.f);
				ControlledPawn->AddActorWorldOffset(Move, true);

				// Clamp camera position
				FVector CurrentLocation = ControlledPawn->GetActorLocation();
				CurrentLocation.X = FMath::Clamp(CurrentLocation.X, OriginPosition.X - 200.f, OriginPosition.X + 200.f);
				CurrentLocation.Y = FMath::Clamp(CurrentLocation.Y, OriginPosition.Y - 200.f, OriginPosition.Y + 200.f);
				CurrentLocation.Z = FMath::Clamp(CurrentLocation.Z, OriginPosition.Z - 200.f, OriginPosition.Z + 200.f);
				ControlledPawn->SetActorLocation(CurrentLocation);

				UE_LOG(LogTemp, Warning, TEXT("PinchZoom Delta=%.2f | Loc=%s"), Delta, *CurrentLocation.ToString());
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
	LastTouchPosition = Value.Get<FVector2D>();
}

void APlayerCameraController::OnTouchReleased(const FInputActionValue& Value)
{
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

	FVector2D Delta = CurrentPosition - LastTouchPosition;
	if (Delta.SizeSquared() < KINDA_SMALL_NUMBER) return;

	LastTouchPosition = CurrentPosition;

	if (APawn* ControlledPawn = GetPawn())
	{
		FVector MoveDirection(Delta.Y * PanSpeed, -Delta.X * PanSpeed, 0.f);
		ControlledPawn->AddActorWorldOffset(MoveDirection, true);

		// Clamp after pan too
		FVector CurrentLocation = ControlledPawn->GetActorLocation();
		CurrentLocation.X = FMath::Clamp(CurrentLocation.X, OriginPosition.X - ZoomMin, OriginPosition.X + ZoomMax);
		CurrentLocation.Y = FMath::Clamp(CurrentLocation.Y, OriginPosition.Y - ZoomMin, OriginPosition.Y + ZoomMax);
		ControlledPawn->SetActorLocation(CurrentLocation);
	}
}

void APlayerCameraController::OnZoomTriggered(const FInputActionValue& Value)
{
	float ZoomValue = Value.Get<float>();
	if (FMath::Abs(ZoomValue) > KINDA_SMALL_NUMBER)
	{
		if (APawn* ControlledPawn = GetPawn())
		{
			FVector Move = ControlledPawn->GetActorForwardVector() * ZoomValue * ZoomSpeed;
			ControlledPawn->AddActorWorldOffset(Move, true);

			// Clamp
			FVector CurrentLocation = ControlledPawn->GetActorLocation();
			CurrentLocation.X = FMath::Clamp(CurrentLocation.X, OriginPosition.X - ZoomMin, OriginPosition.X + ZoomMax);
			CurrentLocation.Y = FMath::Clamp(CurrentLocation.Y, OriginPosition.Y - ZoomMin, OriginPosition.Y + ZoomMax);
			CurrentLocation.Z = FMath::Clamp(CurrentLocation.Z, OriginPosition.Z - ZoomMin, OriginPosition.Z + ZoomMax);
			ControlledPawn->SetActorLocation(CurrentLocation);

			UE_LOG(LogTemp, Warning, TEXT("Zoom Move=%s | Loc=%s"), *Move.ToString(), *CurrentLocation.ToString());
		}
	}
}
