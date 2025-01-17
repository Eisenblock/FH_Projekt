// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "TP_WeaponComponent.h"
#include "MuzzelFlash.generated.h"

class UTP_WeaponComponent;

UCLASS()
class FH_PROJEKT_API AMuzzelFlash : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMuzzelFlash();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MuzzleFlashMesh;

	UTP_WeaponComponent* WeaponComponent;

	void KillObject();
	void GetWeaponComponent();
};
