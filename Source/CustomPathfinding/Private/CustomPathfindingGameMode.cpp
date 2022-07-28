// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomPathfindingGameMode.h"
#include "CustomPathfindingPlayerController.h"
#include "CustomPathfindingCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACustomPathfindingGameMode::ACustomPathfindingGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ACustomPathfindingPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}