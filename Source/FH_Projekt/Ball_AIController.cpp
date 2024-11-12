// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"
#include "Ball_AIController.h"

ABall_AIController::ABall_AIController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABall_AIController::MoveToTargetLocation(const FVector& goal_pos)
{
	MoveToLocation(goal_pos , 5.0f);
}
