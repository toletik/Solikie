// Copyright Epic Games, Inc. All Rights Reserved.

#include "SolikieGameMode.h"
#include "SolikiePlayerController.h"
#include "SolikieCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASolikieGameMode::ASolikieGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ASolikiePlayerController::StaticClass();
	
}