// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FH_ProjektCharacter.h"
#include "ShootTest.generated.h"



UCLASS()
class FH_PROJEKT_API AShootTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShootTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override; 
    
    UPROPERTY(EditAnywhere, Category = "Stats")
    float speed;

private:
    // Komponenten
    UPROPERTY(EditAnywhere)
    USphereComponent* SphereCollider;

    UPROPERTY(EditAnywhere)
    USphereComponent* SphereCollider2;

    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere)
    UNiagaraComponent* explosion;

   

    UPROPERTY(EditAnywhere, Category = "Stats")
    float dmg;

    UPROPERTY(EditAnywhere, Category = "Stats")
    float dmg_partikel;

    UPROPERTY(EditAnywhere, Category = "Stats")
    float explode_time;

    UPROPERTY(EditAnywhere, Category = "Sound")
    USoundBase* explosiv_sound;

    FVector DirectionToPlayer;

    bool bHit;

    float LaunchAngleDegrees = 80.0f;

    FTimerHandle SpeedZeroTimerHandle;
    FTimerHandle DestroyTimerHandle;

    AFH_ProjektCharacter* character;
    TArray<AActor*> FoundActors;

    // Overlap-Handler
    UFUNCTION()
    void OnSphereOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

    UFUNCTION()
    void OnSphereOverlap2(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

 

    void FindPlayerWithTag();
    void Explode();
    void KillObject();
};
