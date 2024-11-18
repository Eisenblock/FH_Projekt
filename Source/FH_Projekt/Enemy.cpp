// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Enemy_Controller.h"
#include "Kismet/GameplayStatics.h"
#include "FH_ProjektCharacter.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h" 
#include "Components/CapsuleComponent.h"
#include "Engine/Engine.h"      
#include "GameFramework/Character.h"
#include "Ball_AIController.h"
#include "Math/UnrealMathUtility.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Tags.Add(FName("Enemy"));
    GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);
    GetCapsuleComponent()->SetVisibility(true);
    life = 100;
    can_die = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
    playerCharacter = Cast<AFH_ProjektCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    charMovement = GetCharacterMovement();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    GetDistanceToPlayer();
    //ChasePlayer();
	EnemyDead();

    FVector PlayerLocation = playerCharacter->GetActorLocation();
    FVector OtherActorLocation = GetActorLocation();

    // Berechne die Entfernung
    float Distance = FVector::Dist(PlayerLocation, OtherActorLocation);

    if (hit_range >= Distance && bCanAttack) 
    {
        Attack();
        bCanAttack = false;

        GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &AEnemy::ResetAttack, attack_speed, false);
    }

    if (aggro_range >= Distance) 
    {
        ChasePlayer();
    }
}

bool AEnemy::GetDmgEnemy(float dmg)
{
	life -= dmg;

    if(life > 0)
    {
   
    
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    
        if (!AnimInstance)   
        {
       
            UE_LOG(LogTemp, Warning, TEXT("AnimInstance ist nullptr!"));        
            return false;
   
        }

   
        if (gotDmg_anim)   
        {
            speed = charMovement->MaxWalkSpeed;
            charMovement->MaxWalkSpeed = 0.0f;
            GetWorld()->GetTimerManager().SetTimer(SpeedTimerHandle, this, &AEnemy::ResetSpeed, 1.0f, false);
            AnimInstance->Montage_Play(gotDmg_anim, 0.7f);
  
        }    
        else 
        {       
            UE_LOG(LogTemp, Warning, TEXT("shoot_anim ist nullptr!")); 
        } 
    }

    if (life <= 0) 
    {
        return true;
    }
	//UE_LOG(LogTemp, Warning, TEXT("Enemy life after damage: %f"), life);
    return false;
}

void AEnemy::EnemyDead()
{
    //charMovement = GetCharacterMovement();

	if (life <= 0 && can_die == false) 
	{
        can_die = true;
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (!AnimInstance)
        {
            UE_LOG(LogTemp, Warning, TEXT("AnimInstance ist nullptr!"));
            return;
        }

        if (death_anim)
        {
            
            charMovement->MaxWalkSpeed = 0.0f;
            GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            AnimInstance->Montage_Play(death_anim, 1.0f);
            int32 RandomNumber = FMath::RandRange(1, 10);
            if (RandomNumber <= 2)
            { 
                spawnPickUpLife();
            }
            else
            {

            }
           
            GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AEnemy::DestroyAfterDelay, 2.0f, false);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("shoot_anim ist nullptr!"));
        }


        //Destroy();
	}
}

void AEnemy::DestroyAfterDelay()
{
    // Zerstöre das Objekt nach dem Timer
    Destroy();
}

void AEnemy::ResetSpeed()
{
    if (life > 0) 
    { 
        charMovement->MaxWalkSpeed = speed;
    }
    else
    {
        charMovement->MaxWalkSpeed = 0;
    }
}

void AEnemy::spawnPickUpLife()
{
    if (pickUpife) // Ensure the subclass is specified
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();

        // Set spawn location and rotation
        FVector SpawnLocation = GetActorLocation() + FVector(10.0f, 0.0f, 0.0f); // Offset to avoid overlap
        FRotator SpawnRotation = GetActorRotation();

        // Spawn the enemy subclass
        AGetLife* SpawnedEnemy = GetWorld()->SpawnActor<AGetLife>(pickUpife, SpawnLocation, SpawnRotation, SpawnParams);

        if (SpawnedEnemy)
        {
            UE_LOG(LogTemp, Log, TEXT("Successfully spawned enemy: %s"), *SpawnedEnemy->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to spawn enemy!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("EnemyClass is not set!"));
    }
}
   

void AEnemy::Attack()
{
   // AFH_ProjektCharacter* playerCharacter = Cast<AFH_ProjektCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if (can_die == false)
    { 
    
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
        charMovement->MaxWalkSpeed = 0;
        GetWorld()->GetTimerManager().SetTimer(SpeedTimerHandle, this, &AEnemy::ResetSpeed, 2.0f, false);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("shoot_anim ist nullptr!"));
    }

    }
   
}

void AEnemy::EnemyGetLife(float life_)
{
    life += life_;
    UE_LOG(LogTemp, Log, TEXT("Aktueller Lebenswert: %f"), life);
}

void AEnemy::GetDistanceToPlayer()
{
   // ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

    // Richtung berechnen
    goal_pos = (playerCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    goal_pos.IsNormalized();
}

void AEnemy::ChasePlayer()
{
    FVector NewLocation = playerCharacter->GetActorLocation();

    // Debug: Aktuelle Position des Balls und Zielposition anzeigen
    //UE_LOG(LogTemp, Warning, TEXT("Current Position: %s"), *GetActorLocation().ToString());
    //UE_LOG(LogTemp, Warning, TEXT("New Location (Target Position): %s"), *NewLocation.ToString());

    if (ABall_AIController* AIController = Cast<ABall_AIController>(GetController()))
    {
        // Beispielziel in Richtung des Spielers
        AIController->MoveToTargetLocation(NewLocation);

        // Debug: Zeigt an, dass MoveToLocation aufgerufen wird
        //UE_LOG(LogTemp, Warning, TEXT("AIController is moving to %s"), *NewLocation.ToString());
    }
}

void AEnemy::ResetAttack()
{
    bCanAttack = true;
}


void AEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this)
    {
        UE_LOG(LogTemp, Log, TEXT("Overlap detected with: %s"), *OtherActor->GetName());
    }
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

