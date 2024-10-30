// Fill out your copyright notice in the Description page of Project Settings.


#include "Waypoint_East.h"

// Sets default values
AWaypoint_East::AWaypoint_East()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(FName("WayPoint_East"));
}

// Called when the game starts or when spawned
void AWaypoint_East::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWaypoint_East::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

