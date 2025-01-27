// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "GetLife.generated.h"

UCLASS()
class FH_PROJEKT_API AGetLife : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGetLife();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	// Method to trigger when player overlaps
	UFUNCTION()
	void PerformActionOnOverlap();

	// Static Mesh Component
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	// Collision Box Component
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CollisionBox;


	//Variable
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	float life_amount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	bool bomb;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	bool heal;
};

