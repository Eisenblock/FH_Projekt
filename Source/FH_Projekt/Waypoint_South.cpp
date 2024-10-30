// Fill out your copyright notice in the Description page of Project Settings.


#include "Waypoint_South.h"

// Sets default values
AWaypoint_South::AWaypoint_South()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add(FName("WayPoint_South"));
}

// Called when the game starts or when spawned
void AWaypoint_South::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWaypoint_South::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

