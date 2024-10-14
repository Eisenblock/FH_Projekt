// Copyright Epic Games, Inc. All Rights Reserved.

#include "FH_ProjektGameMode.h"
#include "FH_ProjektCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFH_ProjektGameMode::AFH_ProjektGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
