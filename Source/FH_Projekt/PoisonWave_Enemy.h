// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "NiagaraComponent.h"
#include "FH_ProjektCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "PoisonWave_Enemy.generated.h"

UCLASS()
class FH_PROJEKT_API APoisonWave_Enemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APoisonWave_Enemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
    // Box Colliders
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
    UBoxComponent* BoxCollider1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
    UBoxComponent* BoxCollider2;

    UPROPERTY(EditAnywhere)
    UNiagaraComponent* wave_partikel;


    // Damage settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
    float Damage1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
    float Damage2;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
    float DamageDuration;

    // Overlap Methods
    UFUNCTION()
    void OnEndOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex
    );

    UFUNCTION()
    void OnBox2Overlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // Timer to stop damage
  
    void DoDamage();
    void DestroyObject();
    void SetCollider2True();
    void FIndPlayer();
    void StopTimer();
    

private:
    FTimerHandle DamageTimerHandle;
    FTimerHandle CastTimerHandle;
    FTimerHandle VisibleTimer;
    FTimerHandle ColliderTimer;

    bool bCanDealDamage;
    float TimeSinceLastUpdate; 
    float UpdateInterval;
    FVector Direction;
    FVector start_pos;

    USkeletalMeshComponent* enemyMesh;
    FRotator rotation;
    AFH_ProjektCharacter* Character;
    bool enemyDead;
    AEnemy* current_Enemy;
};


