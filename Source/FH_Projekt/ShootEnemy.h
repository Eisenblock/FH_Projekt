// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FH_ProjektCharacter.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ShootEnemy.generated.h"

UCLASS()
class FH_PROJEKT_API AShootEnemy : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AShootEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	AFH_ProjektCharacter* character;

	FVector start_Pos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float speed;

	// Kollisionskomponente
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class USphereComponent* CollisionComponent;

	// Mesh für den Ball
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
	class UStaticMeshComponent* BallMesh;

	UPROPERTY(VisibleAnywhere)
	UFloatingPawnMovement* FloatingPawnMovement;

};
