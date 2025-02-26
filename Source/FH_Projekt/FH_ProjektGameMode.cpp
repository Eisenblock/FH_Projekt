// Copyright Epic Games, Inc. All Rights Reserved.

#include "FH_ProjektGameMode.h"
#include "FH_ProjektCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include <Kismet/GameplayStatics.h>

void AFH_ProjektGameMode::BeginPlay()
{
 
}

AFH_ProjektGameMode::AFH_ProjektGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void AFH_ProjektGameMode::LoadLevelAsync(FName LevelName)
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        PlayerController->SetPause(true);
    }



    // Asynchrones Laden starten
    FLatentActionInfo LatentInfo;
    LatentInfo.CallbackTarget = this;
    LatentInfo.ExecutionFunction = "OnLevelLoadComplete";  // Wird aufgerufen, wenn das Level geladen ist
    LatentInfo.Linkage = 0;
    LatentInfo.UUID = __LINE__;

    UGameplayStatics::LoadStreamLevel(this, LevelName, true, false, LatentInfo);

}

void AFH_ProjektGameMode::OnLevelLoadComplete()
{
    UE_LOG(LogTemp, Warning, TEXT("Level vollständig geladen!"));

 

    // Spiel fortsetzen
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        PlayerController->SetPause(false);
    }
}
