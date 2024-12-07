// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <NiagaraFunctionLibrary.h>
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h" 
#include "FH_ProjektCharacter.h"
#include "Projectile_Enemy.generated.h"

UCLASS()
class FH_PROJEKT_API AProjectile_Enemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile_Enemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Launch the projectile
    UFUNCTION(BlueprintCallable, Category = "Projectile")
    void LaunchProjectile(float Strength, float AngleDegrees);

    void Explode();
    void FindPlayerWithTag();
    void Destroy();
    void OverLapDmg();
    void OverLapDmgPartikel();

    //UFUNCTION()
    void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    UPROPERTY(EditAnywhere, Category = "Stats")
    float speed;

    UPROPERTY(EditAnywhere, Category = "Stats")
    float dmg;

    UPROPERTY(EditAnywhere, Category = "Stats")
    float dmg_partikel;

    FVector DirectionToPlayer;

    float LaunchAngleDegrees = 45.0f;

    AFH_ProjektCharacter* character;
    TArray<AActor*> FoundActors;

    UPROPERTY(EditAnywhere, Category = "Effects")
    UNiagaraSystem* ExplosionEffect;

    UPROPERTY(EditAnywhere, Category = "Components")
    UStaticMeshComponent* Mesh;

    UPROPERTY(EditAnywhere,  Category = "Components")
    USphereComponent* SphereCollider;

private:

    UPROPERTY(VisibleAnywhere, Category = "Components")
    class UProjectileMovementComponent* ProjectileMovement;

    UPROPERTY(EditAnywhere, Category = "Components")
    UNiagaraComponent* explosion;

    FTimerHandle ExplosionTimerHandle;

    // Dauer, wie lange die Explosion sichtbar sein soll
    float ExplosionDuration = 1.0f;

    // Zeitverzögerung vor Aktivierung
    float DelayBeforeExplosion = 3.0f;

};
