// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawn.h"
#include "Engine/World.h"
#include "Enemy.h"
#include "Enemy_Controller.h"
#include "TimerManager.h"

// Konstruktor: Standardwerte setzen
AEnemySpawn::AEnemySpawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// Standardwerte initialisieren
	SpawnInterval = 5.0f; // Gegner spawnt alle 5 Sekunden
	MaxEnemies = 10; // Maximal 10 Gegner
	CurrentEnemies = 0; // Start mit 0 Gegnern
}

// Wird aufgerufen, wenn das Spiel beginnt oder der Actor gespawnt wird
void AEnemySpawn::BeginPlay()
{
	Super::BeginPlay();

	// Spawning wird bei Spielstart gestartet
	StartSpawning();
}

// Spawning-Logik
void AEnemySpawn::StartSpawning()
{
	// Timer, der die SpawnEnemy-Funktion regelmäßig aufruft
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemySpawn::SpawnEnemy, SpawnInterval, true);
}

// SpawnEnemy: Spawnt einen Gegner an der Position des Spawners
void AEnemySpawn::SpawnEnemy()
{
    if (EnemyClass) // Stelle sicher, dass die Klasse gesetzt ist
    {
        FVector SpawnLocation = GetActorLocation();
        FRotator SpawnRotation = GetActorRotation();

        // Spawne den Gegner
        AEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyClass, SpawnLocation, SpawnRotation);
        if (SpawnedEnemy)
        {
            // Setze den AIController für den Gegner (wird oft automatisch gemacht, aber sicherheitshalber)
            AEnemy_Controller* AIController = Cast<AEnemy_Controller>(SpawnedEnemy->GetController());

            if (!AIController)
            {
                UE_LOG(LogTemp, Warning, TEXT("AIController was not automatically assigned, assigning manually."));
                AIController = GetWorld()->SpawnActor<AEnemy_Controller>(AEnemy_Controller::StaticClass(), SpawnLocation, SpawnRotation);
                if (AIController)
                {
                    AIController->Possess(SpawnedEnemy); // AIController übernimmt den Gegner
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

// Tick: Wird jede Frame aufgerufen
void AEnemySpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

