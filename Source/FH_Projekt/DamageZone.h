// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "DamageZone.generated.h"



UCLASS()
class FH_PROJEKT_API ADamageZone : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ADamageZone();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;


    // Box Collision Component
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* CollisionBox;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
    class UPointLightComponent* PointLightComponent;

    // Particle System for Effects
    UPROPERTY(EditAnywhere, Category = "Effects")
    UNiagaraSystem* NiagaraEffect;

    UPROPERTY(EditAnywhere, Category = "Effects")
    UNiagaraComponent* NiagaraComponent;

    // Damage Value
    UPROPERTY(EditAnywhere, Category = "Damage")
    float DamageAmount = 10.0f;

    // Damage Interval
    UPROPERTY(EditAnywhere, Category = "Damage")
    float DamageInterval = 2.0f;

    // Timer Handle
    FTimerHandle DamageTimer;

    UNiagaraComponent* ActiveNiagaraComponent;

    // Overlap Functions
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    // Apply Damage
    void ApplyDamage(AActor* PlayerActor);

    void SetActorVisible();
    void SetColllision();
    void CleanColllision();
    void ActivateLight();
    void DeActiveLight();
    void ActivateTrap();
};
