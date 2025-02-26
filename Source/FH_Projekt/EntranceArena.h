// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "FH_ProjektCharacter.h"
#include "EntranceArena.generated.h"

UCLASS()
class FH_PROJEKT_API AEntranceArena : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEntranceArena();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Static Mesh Component
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh;

	// Collision Box Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionBox;

	FTimerHandle timeHandler;
	FTimerHandle timeHandler2;
	AFH_ProjektCharacter* playerCharacter;
	bool movementStart = false;
	bool entranceTriggered = false;
	float deltatimeH = 0.0f;

	// Function to handle overlap events
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	// Function to change collision after delay
	void ChangeCollisionToBlockAll();

	void MoveObjectDown();

	void MoveDownForDuration();

	void StopMovement();

	void MovementTick();
};
