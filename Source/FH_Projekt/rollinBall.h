// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "rollinBall.generated.h"

UCLASS()
class FH_PROJEKT_API ArollinBall : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ArollinBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
    

    // Kollisionskomponente
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    class USphereComponent* CollisionComponent;

    // Mesh für den Ball
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visual")
    class UStaticMeshComponent* BallMesh;

    // Bewegungsgeschwindigkeit
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float ImpulseStrength;

    FVector directionToPlayer;
    FVector start;

    // Overlap-Event Funktion
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    FVector GetDirection() const;

};
