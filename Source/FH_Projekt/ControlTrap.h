// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ControlTrap.generated.h"

UCLASS()
class FH_PROJEKT_API AControlTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AControlTrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	TArray<AActor*> Traps;

	UPROPERTY()
	int32 CurrentTrapIndex = 0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Trap")
	FName trapName = FName("Trap2");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap")
	float trapIntervall = 2.0f;

	int oldValue;

	FTimerHandle TrapActivationTimerHandle;

	UFUNCTION()
	void ActivateNextTrap();
	void DeaktivateAllTraps();
};
