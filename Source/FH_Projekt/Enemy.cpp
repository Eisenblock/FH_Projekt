// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Enemy_Controller.h"
#include "Kismet/GameplayStatics.h"
#include "FH_ProjektCharacter.h"
#include "GameFramework/PlayerController.h"



// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
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

void AEnemy::Attack()
{
    AFH_ProjektCharacter* playerCharacter = Cast<AFH_ProjektCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
   
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    UE_LOG(LogTemp, Warning, TEXT("AttackMethod"));

    // Check if the AnimInstance exists
    if (!AnimInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("AnimInstance ist nullptr!"));
        return;
    }

    // Check if the attack animation montage is assigned
    if (attack_anim)
    {
        AnimInstance->Montage_Play(attack_anim, 1.0f);
        playerCharacter->GetDmg(20);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("shoot_anim ist nullptr!"));
    }
}

void AEnemy::EnemyGetLife(float life_)
{
    life += life_;
    UE_LOG(LogTemp, Log, TEXT("Aktueller Lebenswert: %f"), life);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

