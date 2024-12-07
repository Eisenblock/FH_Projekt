// Fill out your copyright notice in the Description page of Project Settings.


#include "PoisonWave_Enemy.h"
#include "Components/BoxComponent.h"
#include "FH_ProjektCharacter.h"
#include "TimerManager.h"
#include "NiagaraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APoisonWave_Enemy::APoisonWave_Enemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Create and setup Box Collider 1
    BoxCollider1 = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider1"));
    RootComponent = BoxCollider1;
    BoxCollider1->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    BoxCollider1->SetCollisionResponseToAllChannels(ECR_Ignore);
    BoxCollider1->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    BoxCollider1->SetGenerateOverlapEvents(true);

    // Bind overlap event
    BoxCollider1->OnComponentBeginOverlap.AddDynamic(this, &APoisonWave_Enemy::OnBox1Overlap);

    // Create and setup Box Collider 2
    BoxCollider2 = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider2"));
    BoxCollider2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    BoxCollider2->SetCollisionResponseToAllChannels(ECR_Ignore);
    BoxCollider2->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    BoxCollider2->SetGenerateOverlapEvents(true);
    BoxCollider2->SetupAttachment(RootComponent);
    //BoxCollider2->OnComponentBeginOverlap.AddDynamic(this, &APoisonWave_Enemy::OnBox2Overlap);

   
    wave_partikel = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffect"));
    wave_partikel->SetupAttachment(RootComponent);

    // Initialize variables
    Damage1 = 20.0f;
    Damage2 = 30.0f;
    DamageDuration = 5.0f;
    bCanDealDamage = true;
}

// Called when the game starts or when spawned
void APoisonWave_Enemy::BeginPlay()
{
	Super::BeginPlay();
 
    //BoxCollider2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    FIndPlayer();
    //GetDirection();
    BoxCollider1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    BoxCollider1->OnComponentBeginOverlap.AddDynamic(this, &APoisonWave_Enemy::OnBox1Overlap);
    wave_partikel->Activate();
    GetWorld()->GetTimerManager().SetTimer(VisibleTimer, this, &APoisonWave_Enemy::SetCollider2True, 1.0f, false);
    GetWorld()->GetTimerManager().SetTimer(CastTimerHandle, this, &APoisonWave_Enemy::DestroyObject, DamageDuration, false);
    start_pos = GetActorLocation();
}

// Called every frame
void APoisonWave_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
   
    start_pos = enemyMesh->GetSocketLocation("HeadSocket");

    if (current_Enemy)
    {
    
        enemyDead = current_Enemy->GetcanDie();

    if (enemyDead)
    {
        GetWorld()->GetTimerManager().ClearTimer(VisibleTimer);
        GetWorld()->GetTimerManager().ClearTimer(CastTimerHandle);
        Destroy();
    }

    }

    GetDirection();
    

    //start_pos = enemyMesh->GetSocketLocation("HeadSocket");
    //FVector goal_pos = Direction * 200.0f * DeltaTime;
    //start_pos += FVector(50.0f,0.0f,0.0f);

    SetActorLocation(start_pos );
    wave_partikel->SetWorldLocation(start_pos);
}

void APoisonWave_Enemy::OnBox1Overlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag("Player") && bCanDealDamage)
    {
        AFH_ProjektCharacter* character = Cast<AFH_ProjektCharacter>(OtherActor);

        character->GetDmg(Damage1);

        UE_LOG(LogTemp, Warning, TEXT("BoxCollider1: Applied %f damage to %s"), Damage1, *OtherActor->GetName());

        // Start damage cooldown
        bCanDealDamage = false;
        GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, this, &APoisonWave_Enemy::StopDamage, DamageDuration, false);
    }
}

void APoisonWave_Enemy::OnBox2Overlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag("Player") && bCanDealDamage)
    {
        AFH_ProjektCharacter* character = Cast<AFH_ProjektCharacter>(OtherActor);

        character->GetDmg(Damage1);

        UE_LOG(LogTemp, Warning, TEXT("BoxCollider2: Applied %f damage to %s"), Damage2, *OtherActor->GetName());

        // Start damage cooldown
        bCanDealDamage = false;
        //GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, this, &APoisonWave_Enemy::StopDamage, DamageDuration, false);
    }
}

void APoisonWave_Enemy::StopDamage()
{
    bCanDealDamage = true;
    UE_LOG(LogTemp, Warning, TEXT("Damage cooldown reset."));
}

void APoisonWave_Enemy::GetDirection()
{
    

    // Position des Charakters
    FVector CharacterPosition = Character->GetActorLocation() + FVector(50.0f,0.0f,0.0f);

    // Position dieses Objekts
    FVector ObjectPosition = start_pos;

    // Berechne die Richtung (normalisierter Vektor)
    Direction = (CharacterPosition - ObjectPosition).GetSafeNormal();


    // Berechne die Rotation basierend auf der Richtung
    FRotator TargetRotation = Direction.Rotation();

    // Setze die Rotation für das Objekt
    SetActorRotation(TargetRotation);

    // Falls du ein Partikelsystem hast, das ebenfalls in die Richtung zeigen soll:
   

    UE_LOG(LogTemp, Log, TEXT("Direction set towards Character at: %s"), *CharacterPosition.ToString());
}

void APoisonWave_Enemy::DestroyObject()
{
    Destroy();
}

void APoisonWave_Enemy::SetCollider2True()
{
    BoxCollider1->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void APoisonWave_Enemy::GetEnemyPos(AEnemy* enemy, USkeletalMeshComponent* enemy_Mesh)
{
    current_Enemy = enemy;
    enemyMesh = enemy_Mesh;
}

void APoisonWave_Enemy::FIndPlayer()
{
    Character = Cast<AFH_ProjektCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

