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
	virtual void SetupInputComponent() override;

	// Reference to the Input Mapping Context asset
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* TouchPressedAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* TouchPositionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float PanSpeed = 1.5f;

	// Header
private:
	bool bTouchingTwoFingers = false;
	FVector2D Touch0, Touch1;
	float LastTouchDistance = 0.f;
	float ZoomSpeed = 0.01f;

	bool bFirstTouch = false;

	void OnTouchPressed(const FInputActionValue& Value);
	void OnTouchReleased(const FInputActionValue& Value);
	void OnTouchPosition(const FInputActionValue& Value);

	bool bTouching = false;
	FVector2D LastTouchPosition;
};
