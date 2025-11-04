// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCameraController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

void APlayerCameraController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (InputMappingContext)
		{
			Subsystem->AddMappingContext(InputMappingContext, 0 );
		}
	}
	// Show the mouse cursor
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	bEnableTouchEvents = true;

	UE_LOG(LogTemp, Warning, TEXT("BeginPlay called, InputMappingContext: %s"), 
	*GetNameSafe(InputMappingContext));
}

void APlayerCameraController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UE_LOG(LogTemp, Warning, TEXT("SetupInputComponent called"));

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("InputComponent cast to EnhancedInputComponent successful"));

		if (TouchPressedAction)
		{
			UE_LOG(LogTemp, Warning, TEXT("Binding TouchPressedAction"));
			EIC->BindAction(TouchPressedAction, ETriggerEvent::Started, this, &APlayerCameraController::OnTouchPressed);
			UE_LOG(LogTemp, Warning, TEXT("TouchPressedAction bound"));
			EIC->BindAction(TouchPressedAction, ETriggerEvent::Completed, this, &APlayerCameraController::OnTouchReleased);
		}
		if (TouchPositionAction)
		{
			UE_LOG(LogTemp, Warning, TEXT("Binding TouchPositionAction"));
			EIC->BindAction(TouchPositionAction, ETriggerEvent::Triggered, this, &APlayerCameraController::OnTouchPosition);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to cast InputComponent to EnhancedInputComponent"));
	}
	UE_LOG(LogTemp, Warning, TEXT("TouchPressedAction: %s"), *GetNameSafe(TouchPressedAction));
	UE_LOG(LogTemp, Warning, TEXT("TouchPositionAction: %s"), *GetNameSafe(TouchPositionAction));
}

void APlayerCameraController::OnTouchPressed(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Click detected!"));
	bTouching = true;
	bFirstTouch = true;

	FVector2D Position = Value.Get<FVector2D>();
	LastTouchPosition = Position;
}


void APlayerCameraController::OnTouchReleased(const FInputActionValue& Value)
{
	bTouching = false;
}

void APlayerCameraController::OnTouchPosition(const FInputActionValue& Value)
{
	if (!bTouching)
		return;

	FVector2D CurrentPosition = Value.Get<FVector2D>();

	if (bFirstTouch)
	{
		LastTouchPosition = CurrentPosition;
		bFirstTouch = false;
		UE_LOG(LogTemp, Warning, TEXT("First touch tick ignored"));
		return;
	}

	FVector2D Delta = CurrentPosition - LastTouchPosition;
	
	if (Delta.SizeSquared() < KINDA_SMALL_NUMBER)
		return;

	LastTouchPosition = CurrentPosition;

	if (APawn* ControlledPawn = GetPawn())
	{
		FVector MoveDirection = FVector::ZeroVector;

		MoveDirection.X = Delta.Y * PanSpeed;
		MoveDirection.Y = -Delta.X * PanSpeed;

		UE_LOG(LogTemp, Warning, TEXT("MoveDirection: X=%f, Y=%f, Z=%f"), MoveDirection.X, MoveDirection.Y, MoveDirection.Z);

		ControlledPawn->AddActorWorldOffset(MoveDirection, true);
	}
}
