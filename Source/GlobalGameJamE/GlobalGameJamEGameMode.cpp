// Copyright Epic Games, Inc. All Rights Reserved.

#include "GlobalGameJamEGameMode.h"
#include "GlobalGameJamECharacter.h"
#include "UObject/ConstructorHelpers.h"

AGlobalGameJamEGameMode::AGlobalGameJamEGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
