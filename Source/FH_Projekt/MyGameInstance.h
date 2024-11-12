// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FH_PROJEKT_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	// Player's current life value
	UPROPERTY(BlueprintReadWrite, Category = "Player Stats")
	float PlayerLife = 100;

	void Changemap();
};
