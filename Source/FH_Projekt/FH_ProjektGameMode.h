// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FH_ProjektGameMode.generated.h"

UCLASS(minimalapi)
class AFH_ProjektGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:


	virtual void BeginPlay() override;


	AFH_ProjektGameMode();

	void LoadLevelAsync(FName LevelName);

	void OnLevelLoadComplete();
};



