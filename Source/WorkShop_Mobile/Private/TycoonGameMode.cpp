// Fill out your copyright notice in the Description page of Project Settings.


#include "TycoonGameMode.h"
#include "PlayerCameraController.h"
#include "PlayerActor.h"

ATycoonGameMode::ATycoonGameMode()
{
	// Set default pawn class
	DefaultPawnClass = APlayerActor::StaticClass();

	// Set custom player controller
	PlayerControllerClass = APlayerCameraController::StaticClass();
}

