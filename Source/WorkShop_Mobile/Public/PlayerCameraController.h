// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCameraController.generated.h"

/**
 * 
 */
UCLASS()
class WORKSHOP_MOBILE_API APlayerCameraController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupInputComponent() override;

	// Reference to the Input Mapping Context asset
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* TouchPressedAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* TouchPositionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ZoomAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float PanSpeed = 1.5f;

	UPROPERTY(EditAnywhere, Category="Camera")
	float ZoomSpeed = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float ZoomMax = 800.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float ZoomMin = 800.0f;

	// Header
private:
	float LastTouchDistance = 0.f;
	FVector OriginPosition;

	bool bFirstTouch = false;

	void OnTouchPressed(const FInputActionValue& Value);
	void OnTouchReleased(const FInputActionValue& Value);
	void OnTouchPosition(const FInputActionValue& Value);
	void OnZoomTriggered(const FInputActionValue& Value);

	bool bTouching = false;
	FVector2D LastTouchPosition;
};
