// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawn.generated.h"

// Forward Declaration der Gegnerklasse
class AEnemy;

UCLASS()
class FH_PROJEKT_API AEnemySpawn : public AActor
{
	GENERATED_BODY()

public:
	// Konstruktor, der Standardwerte für die Eigenschaften dieses Akteurs setzt
	AEnemySpawn();

protected:
	// Wird aufgerufen, wenn das Spiel beginnt oder der Actor gespawnt wird
	virtual void BeginPlay() override;

	// Timer-Handle für wiederholtes Spawnen
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	FTimerHandle SpawnTimerHandle;

	FTimerHandle UpdateIndexTimerHandle;
	FTimerHandle HealthChangeTimer;

	// Timer-Handle für wiederholtes Spawnen
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	FTimerHandle GetLifeTime_Enemy;

	int32 a = 0;
	int32 b;
	float extralife;

	// Intervall für das Spawnen von Gegnern (in Sekunden)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnInterval;

	// Funktion, die den Gegner spawnt
	void SpawnEnemy(AActor* posSpawn);
	void ColletcWayPoints();
	void StartCountWaypoint();


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waypoints")
	TArray<AActor*> WaypointsArray_North;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waypoints")
	TArray<AActor*> WaypointsArray_East;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waypoints")
	TArray<AActor*> WaypointsArray_West;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waypoints")
	TArray<AActor*> WaypointsArray_South;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	AEnemy* Enemy_FastAF;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	AEnemy* Enemy_SlowAF;

	// Anzahl der maximal zu spawnenden Gegner
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 MaxEnemies;

	int32 spawnCount;
	// Aktuelle Anzahl der gespawnten Gegner
	int32 CurrentEnemies;
	FString CurrentLevelName;
	float LifeValue = 100.0f;

public:
	// Wird jede Frame aufgerufen
	virtual void Tick(float DeltaTime) override;

	// Klasse des Gegners, der gespawnt werden soll
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<AEnemy> EnemyClass;

	// Funktion zum Starten des Spawnens
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void StartSpawning(AEnemy* enemy);

	void StartEnemyGetLife(TSubclassOf<AEnemy> Enemy);
	void EnemyGetLife(TSubclassOf<AEnemy> Enemy);
};

