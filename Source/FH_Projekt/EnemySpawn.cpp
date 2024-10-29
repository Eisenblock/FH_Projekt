// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawn.h"
#include "Engine/World.h"
#include "Enemy.h"
#include "Enemy_Controller.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Konstruktor: Standardwerte setzen
AEnemySpawn::AEnemySpawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// Standardwerte initialisieren
	SpawnInterval = 2.0f; // Gegner spawnt alle 5 Sekunden
	MaxEnemies = 10; // Maximal 10 Gegner
	CurrentEnemies = 0; // Start mit 0 Gegnern
}

// Wird aufgerufen, wenn das Spiel beginnt oder der Actor gespawnt wird
void AEnemySpawn::BeginPlay()
{
	Super::BeginPlay();

    ColletcWayPoints();
	// Spawning wird bei Spielstart gestartet
	StartSpawning(Enemy_SlowAF);
    //EnemyGetLife(EnemyClass);
    //GetWorldTimerManager().SetTimer(UpdateIndexTimerHandle, this, &AEnemySpawn::StartCountWaypoint, 2.0f, true);
}



// Spawning-Logik
void AEnemySpawn::StartSpawning(AEnemy* EnemyInstance)
{
   
	// Timer, der die SpawnEnemy-Funktion regelmäßig aufruft
    GetWorldTimerManager().SetTimer(SpawnTimerHandle, [this]()
        {  
            
            if (WaypointsArray.Num() > 0) // Sicherstellen, dass das Array Elemente enthält
            {
                int32 a = FMath::RandRange(0, WaypointsArray.Num() - 1);
                AActor* SpawnLocation = WaypointsArray[a];
                SpawnEnemy(SpawnLocation); // Rufe die SpawnEnemy-Funktion mit dem Standort auf
            }
        }, SpawnInterval, true);          
    
}

void AEnemySpawn::EnemyGetLife(AEnemy* Enemy)
{
    if (Enemy)
    {
        float LifeValue = 100.0f; // Beispielwert, den du übergeben möchtest

        // Setze den Timer, der alle 20 Sekunden EnemyGetLife aufruft
        GetWorldTimerManager().SetTimer(GetLifeTime_Enemy, [Enemy, LifeValue]()
            {
          
            }, 5.0f, true);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("EnemyInstance ist null."));
    }
}

// SpawnEnemy: Spawnt einen Gegner an der Position des Spawners
void AEnemySpawn::SpawnEnemy(AActor* posSpawn)
{
    if (EnemyClass) // Stelle sicher, dass die Klasse gesetzt ist
    {
        FVector SpawnLocation = posSpawn->GetActorLocation();
        FRotator SpawnRotation = posSpawn->GetActorRotation();

        // Spawne den Gegner
        Enemy_SlowAF = GetWorld()->SpawnActor<AEnemy>(EnemyClass, SpawnLocation, SpawnRotation);
        if (Enemy_SlowAF)
        {
            // Setze den AIController für den Gegner (wird oft automatisch gemacht, aber sicherheitshalber)
            AEnemy_Controller* AIController = Cast<AEnemy_Controller>(Enemy_SlowAF->GetController());

            if (!AIController)
            {
                UE_LOG(LogTemp, Warning, TEXT("AIController was not automatically assigned, assigning manually."));
                AIController = GetWorld()->SpawnActor<AEnemy_Controller>(AEnemy_Controller::StaticClass(), SpawnLocation, SpawnRotation);
                if (AIController)
                {
                    AIController->Possess(Enemy_SlowAF); // AIController übernimmt den Gegner
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to spawn Enemy!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("EnemyClass not set!"));
    }
}

void AEnemySpawn::ColletcWayPoints()
{
    TArray<AActor*> WaypointActors;
    TArray<AActor*> WaypointActors_North;

    // Suche alle Actor-Instanzen mit dem Tag "waypoint"
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("WayPoint"), WaypointActors);
    UE_LOG(LogTemp, Log, TEXT("Anzahl der WayPoint-Akteure gefunden: %d"), WaypointActors.Num());
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("WayPoint_North"), WaypointActors_North);
    UE_LOG(LogTemp, Log, TEXT("Anzahl der WayPoint-Akteure gefunden north: %d"), WaypointActors_North.Num());

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
    this->WaypointsArray_North = WaypointActors_North;
}

void AEnemySpawn::StartCountWaypoint()
{
           int32 x = FMath::RandRange(0, 1);
            a = x;
            UE_LOG(LogTemp, Log, TEXT("Neuer Wert für a: %d"), x)   
}

// Tick: Wird jede Frame aufgerufen
void AEnemySpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

