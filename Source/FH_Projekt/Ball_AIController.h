// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Ball_AIController.generated.h"

/**
 * 
 */
UCLASS()
class FH_PROJEKT_API ABall_AIController : public AAIController
{
	GENERATED_BODY()

public :

	ABall_AIController();
	
	void MoveToTargetLocation(const FVector& goal_pos);
};
