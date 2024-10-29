// Fill out your copyright notice in the Description page of Project Settings.


#include "Waypoint_North.h"

// Sets default values
AWaypoint_North::AWaypoint_North()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(FName("WayPoint_North"));

}

// Called when the game starts or when spawned
void AWaypoint_North::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWaypoint_North::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

