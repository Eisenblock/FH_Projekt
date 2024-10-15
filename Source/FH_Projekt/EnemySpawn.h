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
	FTimerHandle SpawnTimerHandle;

	// Intervall für das Spawnen von Gegnern (in Sekunden)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnInterval;

	// Funktion, die den Gegner spawnt
	void SpawnEnemy();

	// Anzahl der maximal zu spawnenden Gegner
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	int32 MaxEnemies;

	// Aktuelle Anzahl der gespawnten Gegner
	int32 CurrentEnemies;

public:
	// Wird jede Frame aufgerufen
	virtual void Tick(float DeltaTime) override;

	// Klasse des Gegners, der gespawnt werden soll
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<AEnemy> EnemyClass;

	// Funktion zum Starten des Spawnens
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void StartSpawning();
};

