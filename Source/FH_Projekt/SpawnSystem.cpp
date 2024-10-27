// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnSystem.h"
#include "Enemy.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpawnSystem::ASpawnSystem()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnSystem::BeginPlay()
{
	Super::BeginPlay();
	ColletcWayPoints();

}

// Called every frame
void ASpawnSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASpawnSystem::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASpawnSystem::ColletcWayPoints()
{
    TArray<AActor*> WaypointActors;

    // Suche alle Actor-Instanzen mit dem Tag "waypoint"
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("WayPoint"), WaypointActors);

    // Überprüfe, ob Wegpunkte gefunden wurden
    if (WaypointActors.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Keine Wegpunkte mit dem Tag 'waypoint' gefunden."));
        return;
    }

    // Debug-Ausgabe: Anzahl der gefundenen Wegpunkte
    UE_LOG(LogTemp, Log, TEXT("Anzahl gefundener Wegpunkte: %d"), WaypointActors.Num());

    // Speichere die Wegpunkte im Character-Array, wenn du ein Array für sie hast
    this->WaypointsArray = WaypointActors;
}

void ASpawnSystem::SpawnEnemyAtRandomWaypoint(TSubclassOf<AActor> EnemyClass)
{
  
    // Wähle eine zufällige Position aus dem Array
    int32 RandomIndex = FMath::RandRange(0, WaypointsArray.Num() - 1);
    FVector SpawnLocation = WaypointsArray[RandomIndex]->GetActorLocation();

    // Setze die Rotation für den Gegner auf die Standard-Orientierung
    FRotator SpawnRotation = FRotator::ZeroRotator;

    // Spawnparameter festlegen
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    // Spawne den Gegner an der zufälligen Wegpunkt-Position
    AActor* SpawnedEnemy = GetWorld()->SpawnActor<AActor>(EnemyClass, SpawnLocation, SpawnRotation, SpawnParams);
}

void ASpawnSystem::StartSpawning()
{
    
}

