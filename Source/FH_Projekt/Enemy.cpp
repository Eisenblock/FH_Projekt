// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "ShootTest.h"
#include "Enemy_Controller.h"
#include "PoisonWave_Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "FH_ProjektCharacter.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h" 
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Engine.h"      
#include "GameFramework/Character.h"
#include "Ball_AIController.h"
#include "Math/UnrealMathUtility.h"
#include "Projectile_Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemy::AEnemy()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
    Tags.Add(FName("Enemy"));

    GetCapsuleComponent()->SetVisibility(true);
    life = 100;
    can_die = false;
    //GetCharacterMovement()->MaxWalkSpeed = speed;

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollider"));
    SphereComponent->InitSphereRadius(100.0f);
    SphereComponent->SetupAttachment(Mesh1P);
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
    Super::BeginPlay();
    playerCharacter = Cast<AFH_ProjektCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    charMovement = GetCharacterMovement();
    charMovement->MaxWalkSpeed = speed;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    SphereComponent->SetWorldLocation(GetActorLocation());
    GetDistanceToPlayer();
    //ChasePlayer();
    EnemyDead();
  
    FVector PlayerLocation = playerCharacter->GetActorLocation();
    FVector OtherActorLocation = GetActorLocation();
    float Distance = FVector::Dist(PlayerLocation, OtherActorLocation);  

  

    if ((hit_range + 200.0f) > Distance && !checkLineOfSide)
    {
        line_of_side = CheckLineOfSide();
        checkLineOfSide = true;

        GetWorld()->GetTimerManager().SetTimer(LineOFSIdeTimerHandle, this, &AEnemy::ResetLineOfSideTimer, 0.3f, false);
    }

    CheckAttackType();



    if (aggro_range >= Distance && can_Rotate || gotDmgBWalk && can_Rotate)
    {
        isWalking = true;
        ChasePlayer();
    }

}

bool AEnemy::GetDmgEnemy(float dmg_ref, FVector HitLocation, FVector HitNormal)
{
    life -= dmg_ref;
    gotDmgBWalk = true;

    if (life > 0 && !aniInAction && !gotDmgB)
    {
        gotDmgB = true;
        GetWorld()->GetTimerManager().SetTimer(GotDmgTimerHandle,this, &AEnemy::ResetGotDmg,  3.0f, false);

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
            GetWorld()->GetTimerManager().SetTimer(SpeedTimerHandle, this, &AEnemy::ResetSpeed, (gotDmg_anim->GetPlayLength() + 0.5f), false);
            AnimInstance->Montage_Play(gotDmg_anim, 0.7f);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("gotDmg_anim ist nullptr!"));
        }

        // Spawn the Niagara effect at the hit location

    }

    SpawnImpactEffect(HitLocation, HitNormal.Rotation());

    if (life <= 0)
    {
        return true;
    }
    return false;
}

void AEnemy::EnemyDead()
{
    //charMovement = GetCharacterMovement();

    if (life <= 0 && can_die == false)
    {
        can_die = true;
        if (Projectile) {
            Projectile->SetEnemyDead();

        }
        
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
            SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            AnimInstance->Montage_Play(death_anim, 1.0f);
            int32 RandomNumber = FMath::RandRange(1, 30);
            if (RandomNumber <= 3)
            {
                spawnPickUpLife();
            }
            
            if (RandomNumber == 5 && labEnemy)
            {
                spawnPickBomb();
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
        can_Rotate = true;
    }
    else
    {
        charMovement->MaxWalkSpeed = 0;
    }
}

void AEnemy::ZeroSpeed()
{
    charMovement->MaxWalkSpeed = 0;
}

void AEnemy::spawnPickUpLife()
{
    if (pickUpife) // Ensure the subclass is specified
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();

        // Set spawn location and rotation
        FVector SpawnLocation = GetActorLocation() + FVector(0.0f, 0.0f, 0.0f); // Offset to avoid overlap
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

void AEnemy::spawnPickBomb()
{
    if (pickBomb) // Ensure the subclass is specified
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();

        // Set spawn location and rotation
        FVector SpawnLocation = GetActorLocation() + FVector(0.0f, 0.0f, 0.0f); // Offset to avoid overlap
        FRotator SpawnRotation = GetActorRotation();

        // Spawn the enemy subclass
        AGetLife* SpawnedEnemy = GetWorld()->SpawnActor<AGetLife>(pickBomb, SpawnLocation, SpawnRotation, SpawnParams);

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

void AEnemy::TrackEnemyType()
{

}

void AEnemy::ResetLineOfSideTimer()
{
    checkLineOfSide = false;
}

void AEnemy::ResetGotDmg()
{
    
}

bool AEnemy::GetcanDie()
{
    return can_die;
}

void AEnemy::SpawnImpactEffect(FVector HitLocation, FRotator HitRotation)
{
    if (ImpactEffectNiagara != nullptr)
    {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            ImpactEffectNiagara,
            HitLocation,
            HitRotation
        );
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ImpactEffectNiagara is nullptr! Please assign it in the editor."));
    }
}


void AEnemy::Attack()
{
    // AFH_ProjektCharacter* playerCharacter = Cast<AFH_ProjektCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));


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
        AnimInstance->Montage_Play(attack_anim, 1.25f);
        // playerCharacter->GetDmg(20);
        charMovement->MaxWalkSpeed = 300.0f;
        //GetWorld()->GetTimerManager().SetTimer(SpeedZeroTimerHandle, this, &AEnemy::ZeroSpeed, 0.3f, false);
        GetWorld()->GetTimerManager().SetTimer(SpeedTimerHandle, this, &AEnemy::ResetSpeed, 1.5f, false);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("shoot_anim ist nullptr!"));
    }

    if (attack_sound != nullptr)
    {
        UGameplayStatics::PlaySoundAtLocation(this, attack_sound, GetActorLocation());
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

    FVector EnemyLocation = GetActorLocation();

    // Berechne die Richtung zum Spieler
    DirectionToPlayer = (NewLocation - EnemyLocation).GetSafeNormal();

    // Berechne die Rotation basierend auf der Richtung
    FRotator TargetRotation = DirectionToPlayer.Rotation();

    // Setze die Rotation des Gegners
    SetActorRotation(FRotator(0.0f, TargetRotation.Yaw, 0.0f));

    // Debug: Aktuelle Position des Balls und Zielposition anzeigen
    //UE_LOG(LogTemp, Warning, TEXT("Current Position: %s"), *GetActorLocation().ToString());
    //UE_LOG(LogTemp, Warning, TEXT("New Location (Target Position): %s"), *NewLocation.ToString());

    if (ABall_AIController* AIController = Cast<ABall_AIController>(GetController()))
    {
        // Beispielziel in Richtung des Spielers
       // AIController->MoveToTargetLocation(NewLocation);
        AIController->MoveToLocation(NewLocation, (hit_range - 50.0f));
        // Debug: Zeigt an, dass MoveToLocation aufgerufen wird
        //UE_LOG(LogTemp, Warning, TEXT("AIController is moving to %s"), *NewLocation.ToString());
    }

    isWalking = true;
}

void AEnemy::ResetAttack()
{
    bCanAttack = true;
    checkLineOfSide = false;
}

void AEnemy::RangeAttack()
{
    if (shoot2) // Überprüfen, ob eine gültige Klasse gesetzt ist
    {
        // Position und Rotation des Projektils bestimmen
        FVector SpawnLocation = GetActorLocation() + FVector(0.0f, 0.0f, 0.0f);
        FRotator SpawnRotation = GetActorRotation();

        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        UE_LOG(LogTemp, Warning, TEXT("AttackMethod"));

        // Check if the AnimInstance exists
        if (!AnimInstance)
        {
            UE_LOG(LogTemp, Warning, TEXT("AnimInstance ist nullptr!"));
            return;
        }

        if (attackrange_anim)
        {
            AnimInstance->Montage_Play(attackrange_anim, 1.0f);
        }

        // Projektil in der Welt spawnen
        AShootTest* ProjectileShoot = GetWorld()->SpawnActor<AShootTest>(shoot2, SpawnLocation, SpawnRotation);
        float dist = FVector::Dist(GetActorLocation(), playerCharacter->GetActorLocation());
        if(dist <= 1200.0f)
        {
            ProjectileShoot->speed = (ProjectileShoot->speed / 2);
        }
        


        /* if (Projectile)
         {
             // Optional: Zusätzliche Konfiguration des Projektils
             Projectile->LaunchProjectile(1000.0f, 45.0f); // Stärke und Winkel
         }*/
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ShootClass is not set!"));
    }
}

void AEnemy::WaveAttack()
{
    if (shoot2) // Überprüfen, ob eine gültige Klasse gesetzt ist
    {

        aniInAction = true;

        can_Rotate = false;
        // Position und Rotation des Projektils bestimmen
        USkeletalMeshComponent* MeshComp = GetMesh();
        FVector SpawnLocation = GetActorLocation() + DirectionToPlayer * 150.0f;
        FRotator SpawnRotation = GetActorRotation();

        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        UE_LOG(LogTemp, Warning, TEXT("AttackMethod"));

        // Check if the AnimInstance exists
        if (!AnimInstance)
        {
            UE_LOG(LogTemp, Warning, TEXT("AnimInstance ist nullptr!"));
            return;
        }

        if (attackwave_anim)
        {
            AnimInstance->Montage_Play(attackwave_anim, 1.25f);
        }

        // Projektil in der Welt spawnen
        Projectile = GetWorld()->SpawnActor<APoisonWave_Enemy>(wave, SpawnLocation, SpawnRotation);
        
        //GetWorld()->GetTimerManager().SetTimer(SpeedZeroTimerHandle,this,&AEnemy::ResetSpeed,2.0f,false);

        /* if (Projectile)
         {
             // Optional: Zusätzliche Konfiguration des Projektils
             Projectile->LaunchProjectile(1000.0f, 45.0f); // Stärke und Winkel
         }*/
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ShootClass is not set!"));
    }

    if (attack2_sound != nullptr)
    {
        UGameplayStatics::PlaySoundAtLocation(this, attack2_sound, GetActorLocation());
    }
}

void AEnemy::ExplosionAttack()
{
    aniInAction = true;

    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

    if (!AnimInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("AnimInstance ist nullptr!"));
        return;
    }

    if (dance_anim)
    {
        AnimInstance->Montage_Play(dance_anim, 1.25f);
        // playerCharacter->GetDmg(20);
        charMovement->MaxWalkSpeed = 0;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("shoot_anim ist nullptr!"));
    }

    GetWorld()->GetTimerManager().SetTimer(ColliderTimerHandle, this, &AEnemy::ActivColliderExplode, 2.0f, false);

}

void AEnemy::ActivColliderExplode()
{
    if (!can_die) {
        SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

        FVector EffectLocation = GetActorLocation();
        FRotator EffectRotation = FRotator::ZeroRotator;

        // Niagara-Effekt spawnen (falls gesetzt)
        if (ExplosionEffect)
        {
            UNiagaraFunctionLibrary::SpawnSystemAtLocation(
                GetWorld(),
                ExplosionEffect,
                EffectLocation,
                EffectRotation
            );
        }

        life = 0.0f;
        if (explosiv_sound != nullptr)
        {
            UGameplayStatics::PlaySoundAtLocation(this, explosiv_sound, GetActorLocation());
        }
        // GetWorld()->GetTimerManager().SetTimer(ColliderTimerHandle, this, &AEnemy::EnemyDead, 3.0f, false);
    }
}

bool AEnemy::CheckLineOfSide()
{
    FVector Start = GetActorLocation();
    //Start.Y += 50.0f;



    FVector ForwardVector = GetActorForwardVector();
    FVector End = Start + (ForwardVector * 4000.0f);


    FHitResult HitResult;
    FCollisionQueryParams TraceParams;
    TraceParams.bTraceComplex = true;
    TraceParams.AddIgnoredActor(this);


    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        Start,
        End,
        ECC_Visibility,
        TraceParams
    );


    //DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f);


    if (bHit && HitResult.GetActor())
    {
        UE_LOG(LogTemp, Log, TEXT("Spieler in Sichtlinie gefunden!"));

        if (HitResult.GetActor()->ActorHasTag(FName("Player")))
        {
            UE_LOG(LogTemp, Log, TEXT("Spieler in Sichtlinie gefunden!"));
            return true;
        }
    }

    // Kein Spieler getroffen
    UE_LOG(LogTemp, Log, TEXT("kein Spieler in Sichtlinie gefunden!"));
    return false;
}

void AEnemy::CheckAttackType()
{
    FVector PlayerLocation = playerCharacter->GetActorLocation();
    FVector OtherActorLocation = GetActorLocation();

    float Distance = FVector::Dist(PlayerLocation, OtherActorLocation);


    if (zombie_sound && Distance <= 200.0f && !playZombieSound) {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), zombie_sound, PlayerLocation);
        playZombieSound = true;
    }

    if (line_of_side)
    {

        if (hit_range >= Distance && rangeEnemy)
        {
            GetWorld()->GetTimerManager().SetTimer(SpeedTimerHandle, this, &AEnemy::ResetSpeed, 1.0f, false);
        }

        if (hit_range >= Distance && waveEnemy)
        {
            GetWorld()->GetTimerManager().SetTimer(SpeedTimerHandle, this, &AEnemy::ResetSpeed, 3.0f, false);
        }

        if (hit_range >= Distance && bCanAttack && meeleEnemy && !can_die)
        {

            Attack();
            bCanAttack = false;
            GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &AEnemy::ResetAttack, attack_speed, false);


        }


        if (hit_range >= Distance && bCanAttack && rangeEnemy && !can_die)
        {
            RangeAttack();
            bCanAttack = false;
            charMovement->MaxWalkSpeed = 0;


            GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &AEnemy::ResetAttack, attack_speed, false);
        }

        if (hit_range >= Distance && bCanAttack && explodeEnemy && !can_die)
        {
            ExplosionAttack();
            bCanAttack = false;
            charMovement->MaxWalkSpeed = 0;


            //GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &AEnemy::ResetAttack, attack_speed, false);
        }


        if (hit_range >= Distance && bCanAttack && waveEnemy && !can_die)
        {
            WaveAttack();
            bCanAttack = false;
            charMovement->MaxWalkSpeed = 0;

            GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &AEnemy::ResetAttack, attack_speed, false);
        }
    }
}


void AEnemy::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this && OtherActor->ActorHasTag(FName("Player")))
    {
        AFH_ProjektCharacter* character = Cast<AFH_ProjektCharacter>(OtherActor);
        if (character)
        {
            character->GetDmg(20);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("OtherActor konnte nicht in AFH_ProjektCharacter gecastet werden!"));
        }
    }
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

