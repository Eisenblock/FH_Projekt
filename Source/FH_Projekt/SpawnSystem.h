// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpawnSystem.generated.h"

UCLASS()
class FH_PROJEKT_API ASpawnSystem : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASpawnSystem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ColletcWayPoints();
	void SpawnEnemyAtRandomWaypoint(TSubclassOf<AActor> EnemyClass);
	void  StartSpawning();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Waypoints")
	TArray<AActor*> WaypointsArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	AEnemy* Enemy_FastAF;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	AEnemy* Enemy_SlowAF;
};
