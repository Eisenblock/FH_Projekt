// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Enemy_Controller.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Tags.Add(FName("Enemy"));
	
	
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	EnemyDead();
}

void AEnemy::GetDmgEnemy(float dmg)
{
	life -= 100;
	UE_LOG(LogTemp, Warning, TEXT("Enemy life after damage: %f"), life);
}

void AEnemy::EnemyDead()
{
	if (life <= 0) 
	{
		Destroy();
	}
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

