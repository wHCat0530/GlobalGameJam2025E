// Copyright Epic Games, Inc. All Rights Reserved.

#include "GlobalGameJam2025GameMode.h"
#include "GlobalGameJam2025PlayerController.h"
#include "GlobalGameJam2025Character.h"
#include "UObject/ConstructorHelpers.h"

AGlobalGameJam2025GameMode::AGlobalGameJam2025GameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AGlobalGameJam2025PlayerController::StaticClass();

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