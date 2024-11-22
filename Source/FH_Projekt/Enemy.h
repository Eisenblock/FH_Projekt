// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FH_ProjektCharacter.h"
#include "GetLife.h"
#include "Enemy.generated.h"

UCLASS()
class FH_PROJEKT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool GetDmgEnemy(float dmg, FVector HitLocation, FVector HitNormal);
	void EnemyDead();
	void Attack();
	void EnemyGetLife(float life_);
	void GetDistanceToPlayer();
	void ChasePlayer();
	void ResetAttack();
	void DestroyAfterDelay();
	void ResetSpeed();
	void spawnPickUpLife();

	void SpawnImpactEffect(FVector HitLocation, FRotator HitRotation);


	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float life;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	float speed;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float attack_speed;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float aggro_range;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float hit_range;

	FVector goal_pos;
	bool bCanAttack = true;
	bool can_die = false;
	bool rangeEnemy = false;
	FTimerHandle AttackTimerHandle;
	FTimerHandle DestroyTimerHandle;
	FTimerHandle SpeedTimerHandle;

	AFH_ProjektCharacter* playerCharacter;
	UCharacterMovementComponent* charMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* attack_anim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* death_anim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* gotDmg_anim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	TSubclassOf<AGetLife> pickUpife;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UNiagaraSystem* ImpactEffectNiagara;
};
