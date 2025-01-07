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

    

    

    // Create and setup Box Collider 2
    BoxCollider2 = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider2"));
    BoxCollider2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    BoxCollider2->SetCollisionResponseToAllChannels(ECR_Overlap);
    BoxCollider2->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    BoxCollider2->SetGenerateOverlapEvents(true);
    BoxCollider2->OnComponentBeginOverlap.AddDynamic(this, &APoisonWave_Enemy::OnBox2Overlap);
    BoxCollider2->OnComponentEndOverlap.AddDynamic(this, &APoisonWave_Enemy::OnEndOverlap);

    // Set BoxCollider2 as the root component
    RootComponent = BoxCollider2;

    // Create and setup the Niagara particle system
    wave_partikel = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffect"));
    wave_partikel->SetupAttachment(BoxCollider2);

    // Initialize variables
    Damage1 = 5.0f;
    Damage2 = 30.0f;
    DamageDuration = 4.0f;
    bCanDealDamage = true;
}

// Called when the game starts or when spawned
void APoisonWave_Enemy::BeginPlay()
{
	Super::BeginPlay();
 
    //BoxCollider2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    FIndPlayer();
    //GetDirection();
    //BoxCollider2->OnComponentBeginOverlap.AddDynamic(this, &APoisonWave_Enemy::OnBox1Overlap);
    GetWorld()->GetTimerManager().SetTimer(VisibleTimer, this, &APoisonWave_Enemy::SetCollider2True, 1.0f, false);
    GetWorld()->GetTimerManager().SetTimer(CastTimerHandle, this, &APoisonWave_Enemy::DestroyObject, DamageDuration, false);
    start_pos = GetActorLocation();
    SetActorLocation(start_pos);
    wave_partikel->SetWorldLocation(start_pos);
    wave_partikel->Deactivate();
    
}

// Called every frame
void APoisonWave_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
   
    //start_pos = enemyMesh->GetSocketLocation("HeadSocket");

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

    //GetDirection();
    

    //start_pos = enemyMesh->GetSocketLocation("HeadSocket");
    //FVector goal_pos = Direction * 200.0f * DeltaTime;
    //start_pos += FVector(50.0f,0.0f,0.0f);

  // SetActorLocation(start_pos );
  // wave_partikel->SetWorldLocation(start_pos);
}

void APoisonWave_Enemy::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    StopTimer();
}

void APoisonWave_Enemy::OnBox2Overlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag("Player") && bCanDealDamage)
    {
        AFH_ProjektCharacter* character = Cast<AFH_ProjektCharacter>(OtherActor);

        character->GetDmg(Damage1);

        UE_LOG(LogTemp, Warning, TEXT("BoxCollider2: Applied %f damage to %s"), Damage2, *OtherActor->GetName());

        
        GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, this,&APoisonWave_Enemy::DoDamage, 1.0f, true);
        //GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, this, &APoisonWave_Enemy::StopDamage, DamageDuration, false);
    }
}

void APoisonWave_Enemy::DoDamage()
{
    Character->GetDmg(Damage1);
}

void APoisonWave_Enemy::DestroyObject()
{
    Destroy();
}

void APoisonWave_Enemy::SetCollider2True()
{
    BoxCollider2->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    

    //FRotator TargetRotation = Direction.Rotation();
    //TargetRotation.Pitch += 90.0f;

    wave_partikel->Activate();
    //wave_partikel->SetWorldRotation(TargetRotation);
}

void APoisonWave_Enemy::FIndPlayer()
{
    Character = Cast<AFH_ProjektCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void APoisonWave_Enemy::StopTimer()
{
    GetWorld()->GetTimerManager().ClearTimer(DamageTimerHandle);
}



