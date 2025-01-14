// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyGameInstance.h"
#include "EntranceArena.h"
#include "Portal.h"
#include "Components/BoxComponent.h"
#include "EnemySpawn.generated.h"

// Forward Declaration der Gegnerklasse
class AEnemy;
class AFH_ProjektCharacter;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	UBoxComponent* colliderSpawn;

	FTimerHandle UpdateIndexTimerHandle;
	FTimerHandle HealthChangeTimer;
	FTimerHandle ActivationTimer;
	FTimerHandle Block_ActivationTimer;
	

	int32 CurrentIndex = 0;
	int32 CurrentIndex2 = 0;

	// Timer-Handle für wiederholtes Spawnen
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	FTimerHandle GetLifeTime_Enemy;

	float a = 0;
	int32 b;
	float distance;
	bool activatelvl = false;
	int controllloop = 0;
	float extralife;
	bool changeMap = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float timer_ChangeMap ;

	// Intervall für das Spawnen von Gegnern (in Sekunden)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float SpawnInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	float DamageArea_timer;

	// Funktion, die den Gegner spawnt
	void SpawnEnemy(AActor* posSpawn);
	void ColletcWayPoints();
	void StartCountWaypoint();
	void DelayMapChange();
	void SetDamageArea();
	void SpawnPortal();
	void BlockEntrance();
	bool GetLVLisActive();

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);


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



	int32 spawnCount = 0;
	AFH_ProjektCharacter* characterPlayer;
	// Aktuelle Anzahl der gespawnten Gegner
	int32 CurrentEnemies;
	FString CurrentLevelName;
	float LifeValue = 100.0f;
	FTimerHandle DelayTimerHandle;
	float timer = 0; 
	bool bTriggeredClearTimers = false; 
	bool bTriggeredChangeMap = false;

public:
	// Wird jede Frame aufgerufen
	virtual void Tick(float DeltaTime) override;

	UMyGameInstance* MyGameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TArray<TSubclassOf<AEnemy>> EnemyClasses;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<APortal> portal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<AEntranceArena> wallEntrance;



	// Funktion zum Starten des Spawnens
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void StartSpawning(AEnemy* enemy);

	void StartEnemyGetLife(TSubclassOf<AEnemy> Enemy);
	void EnemyGetLife(TSubclassOf<AEnemy> Enemy);
};

