// Fill out your copyright notice in the Description page of Project Settings.


#include "Waypoint_West.h"

// Sets default values
AWaypoint_West::AWaypoint_West()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(FName("WayPoint_West"));
}

// Called when the game starts or when spawned
void AWaypoint_West::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWaypoint_West::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
