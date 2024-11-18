// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawn.h"
#include "Engine/World.h"
#include "Enemy.h"
#include "Enemy_Controller.h"
#include "Kismet/GameplayStatics.h"
#include "FH_ProjektCharacter.h"
#include "MyGameInstance.h"
#include "Ball_AIController.h"
#include "TimerManager.h"

// Konstruktor: Standardwerte setzen
AEnemySpawn::AEnemySpawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// Standardwerte initialisieren
	//SpawnInterval = 2.0f; // Gegner spawnt alle 5 Sekunden
	MaxEnemies = 10; // Maximal 10 Gegner
	CurrentEnemies = 0; // Start mit 0 Gegnern
}

// Wird aufgerufen, wenn das Spiel beginnt oder der Actor gespawnt wird
void AEnemySpawn::BeginPlay()
{
	Super::BeginPlay();
    CurrentLevelName = GetWorld()->GetMapName();
    CurrentLevelName = FPaths::GetBaseFilename(CurrentLevelName);
    ColletcWayPoints();
	// Spawning wird bei Spielstart gestartet
    GetWorldTimerManager().SetTimer(UpdateIndexTimerHandle, this, &AEnemySpawn::StartCountWaypoint, 15.0f, true);
	StartSpawning(Enemy_SlowAF);
    //StartEnemyGetLife(EnemyClass);
    
    UWorld* World = GetWorld();
    if (World)
    {
        MyGameInstance = Cast<UMyGameInstance>(World->GetGameInstance());
        if (MyGameInstance)
        {
           
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("MyGameInstance is not valid!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("GetWorld() is invalid!"));
    }
}



// Spawning-Logik
void AEnemySpawn::StartSpawning(AEnemy* EnemyInstance)
{
   
	// Timer, der die SpawnEnemy-Funktion regelmäßig aufruft
    GetWorldTimerManager().SetTimer(SpawnTimerHandle, [this]()
        {  
            
            if (WaypointsArray_North.Num() > 0 && b == 0) // Sicherstellen, dass das Array Elemente enthält
            {
                int32 a = FMath::RandRange(0, WaypointsArray_North.Num() - 1);
                AActor* SpawnLocation = WaypointsArray_North[a];
                SpawnEnemy(SpawnLocation); // Rufe die SpawnEnemy-Funktion mit dem Standort auf
                UE_LOG(LogTemp, Warning, TEXT("North"));
            }
            if (WaypointsArray_East.Num() > 0 && b == 1) 
            {
                int32 a = FMath::RandRange(0, WaypointsArray_East.Num() - 1);
                AActor* SpawnLocation = WaypointsArray_East[a];
                SpawnEnemy(SpawnLocation);
                UE_LOG(LogTemp, Warning, TEXT("East"));
            }
            if (WaypointsArray_South.Num() > 0 && b == 2) // Sicherstellen, dass das Array Elemente enthält
            {
                int32 a = FMath::RandRange(0, WaypointsArray_South.Num() - 1);
                AActor* SpawnLocation = WaypointsArray_South[a];
                SpawnEnemy(SpawnLocation); // Rufe die SpawnEnemy-Funktion mit dem Standort auf
                UE_LOG(LogTemp, Warning, TEXT("South"));
            }
            if (WaypointsArray_West.Num() > 0 && b == 3)
            {
                int32 a = FMath::RandRange(0, WaypointsArray_West.Num() - 1);
                AActor* SpawnLocation = WaypointsArray_West[a];
                SpawnEnemy(SpawnLocation);
                UE_LOG(LogTemp, Warning, TEXT("West"));
            }
        }, SpawnInterval, true);          
    
}

void AEnemySpawn::StartEnemyGetLife(TSubclassOf<AEnemy> Enemy)
{
   
}

void AEnemySpawn::EnemyGetLife(TSubclassOf<AEnemy> Enemy)
{   
    UE_LOG(LogTemp, Log, TEXT("Enemy versucht mehr Leben"));
    extralife += 100.0f;

                /*if (Enemy)
                {
                    // Zugriff auf das Default-Objekt der Klasse AEnemy
                    AEnemy* DefaultEnemy = Enemy->GetDefaultObject<AEnemy>();
                    if (DefaultEnemy)
                    {
                        // Das Standardleben (z. B. Health) ändern
                        DefaultEnemy->life += 100.0f;  // Angenommen, Health ist eine öffentliche Variable oder hat eine Setter-Methode
                        UE_LOG(LogTemp, Log, TEXT("Neues Standardleben für AEnemy-Klasse gesetzt: %f"), DefaultEnemy->life);
                    }
                }*/                 
}

// SpawnEnemy: Spawnt einen Gegner an der Position des Spawners
void AEnemySpawn::SpawnEnemy(AActor* posSpawn)
{
    
    if (CurrentLevelName != "Testmap" && spawnCount != MaxEnemies)
    {
        if (EnemyClasses.Num() > 0) // Ensure there are enemy classes set
        {
            // Randomly select an enemy class from the array
            int32 RandomIndex = FMath::RandRange(1, 10);
            TSubclassOf<AEnemy> SelectedEnemyClass = EnemyClasses[0];
         
            if (RandomIndex <= 2)
            {
                SelectedEnemyClass = EnemyClasses[0];
            }
            else
            {
                SelectedEnemyClass = EnemyClasses[1];
            }


            if (SelectedEnemyClass)
            {
                FVector SpawnLocation = posSpawn->GetActorLocation();
                FRotator SpawnRotation = posSpawn->GetActorRotation();

                // Spawn the enemy
                Enemy_SlowAF = GetWorld()->SpawnActor<AEnemy>(SelectedEnemyClass, SpawnLocation, SpawnRotation);
                spawnCount += 1;

                if (Enemy_SlowAF)
                {
                    // Set the AIController for the enemy
                    ABall_AIController* AIController = Cast<ABall_AIController>(Enemy_SlowAF->GetController());

                    if (!AIController)
                    {
                        UE_LOG(LogTemp, Warning, TEXT("AIController was not automatically assigned, assigning manually."));
                        AIController = GetWorld()->SpawnActor<ABall_AIController>(ABall_AIController::StaticClass(), SpawnLocation, SpawnRotation);
                        if (AIController)
                        {
                            AIController->Possess(Enemy_SlowAF); // AIController takes control of the enemy
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
                UE_LOG(LogTemp, Error, TEXT("SelectedEnemyClass is invalid!"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("No enemy classes set in EnemyClasses array!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Spawning conditions not met or max enemies reached!"));
    }
}

void AEnemySpawn::ColletcWayPoints()
{
    TArray<AActor*> WaypointActors_South;
    TArray<AActor*> WaypointActors_North;
    TArray<AActor*> WaypointActors_West;
    TArray<AActor*> WaypointActors_East;

    // Suche alle Actor-Instanzen mit dem Tag "waypoint"
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("WayPoint_North"), WaypointActors_North);
    UE_LOG(LogTemp, Log, TEXT("Anzahl der WayPoint-Akteure gefunden North: %d"), WaypointActors_North.Num());
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("WayPoint_East"), WaypointActors_East);
    UE_LOG(LogTemp, Log, TEXT("Anzahl der WayPoint-Akteure gefunden East: %d"), WaypointActors_East.Num());
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("WayPoint_South"), WaypointActors_South);
    UE_LOG(LogTemp, Log, TEXT("Anzahl der WayPoint-Akteure gefunden South: %d"), WaypointActors_South.Num());
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("WayPoint_West"), WaypointActors_West);
    UE_LOG(LogTemp, Log, TEXT("Anzahl der WayPoint-Akteure gefunden West: %d"), WaypointActors_West.Num());

    /*// Überprüfe, ob Wegpunkte gefunden wurden
    if (WaypointActors.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Keine Wegpunkte mit dem Tag 'waypoint' gefunden."));
        return;
    }*/

    // Debug-Ausgabe: Anzahl der gefundenen Wegpunkte
    //UE_LOG(LogTemp, Log, TEXT("Anzahl gefundener Wegpunkte: %d"), WaypointActors.Num());

    // Speichere die Wegpunkte im Character-Array, wenn du ein Array für sie hast
    this->WaypointsArray_North = WaypointActors_North;
    this->WaypointsArray_East = WaypointActors_East;
    this->WaypointsArray_South = WaypointActors_South;
    this->WaypointsArray_West = WaypointActors_West;

}

void AEnemySpawn::StartCountWaypoint()
{
           
            if (SpawnInterval >= 0.1f) 
            { 
                SpawnInterval -= 0.05f;
            }
           
            
}

void AEnemySpawn::DelayMapChange()
{
    MyGameInstance->Changemap();
    UE_LOG(LogTemp, Error, TEXT("Do it"));
}

// Tick: Wird jede Frame aufgerufen
void AEnemySpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    FTimerHandle UnusedHandle;
    if (spawnCount == MaxEnemies && changeMap)
    {
      
                if (MyGameInstance)
                {
                    changeMap = false;
                    MyGameInstance->Changemap();
                    
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("MyGameInstance is nullptr inside the timer lambda."));
                }
           

    }
}

