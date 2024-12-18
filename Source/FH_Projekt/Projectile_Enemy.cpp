// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile_Enemy.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include <NiagaraFunctionLibrary.h>
#include "NiagaraSystem.h"
#include "Components/SphereComponent.h" 
#include "TimerManager.h"

// Sets default values
AProjectile_Enemy::AProjectile_Enemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;
    
    

    // Sphere Collider erstellen
    SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
    /*SphereCollider->SetSphereRadius(50.0f); // Radius anpassen (in Unreal-Einheiten)
    SphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // Nur für Queries wie Overlaps
    SphereCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    //SphereCollider->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    SphereCollider->SetGenerateOverlapEvents(true);*/ // Overlap-Events aktivieren
    SphereCollider->SetupAttachment(Mesh);
    
    
    

    // Create and set up the projectile movement component
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->bShouldBounce = false; // Optional: Set to true if bouncing is desired
    ProjectileMovement->ProjectileGravityScale = 1.0f; // Default gravity

    explosion = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Particelsystem"));
   
    SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &AProjectile_Enemy::OnComponentBeginOverlap);
    
}

// Called when the game starts or when spawned
void AProjectile_Enemy::BeginPlay()
{
	Super::BeginPlay();
   

   
   FindPlayerWithTag();
  
   SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &AProjectile_Enemy::OnComponentBeginOverlap);

   if (explosion)
   {  
       explosion->Deactivate();
   }

  // GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &AProjectile_Enemy::Explode, DelayBeforeExplosion, false);

   // Position des Charakters
   FVector PlayerPosition = character->GetActorLocation(); // Angenommen, du hast einen TargetActor

   // Aktuelle Position des Projektils
   FVector CurrentPosition = GetActorLocation();

   // Berechne die Richtung zum Spieler
   DirectionToPlayer = (PlayerPosition - CurrentPosition).GetSafeNormal(); // Normalisierter Vektor
}

// Called every frame
void AProjectile_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
   

    // Aktuelle Position des Projektils
    FVector CurrentPosition = GetActorLocation();

    SphereCollider->SetRelativeLocation(CurrentPosition);

    if (explosion)
    {
        explosion->SetWorldLocation(CurrentPosition);
    }

   
    float LaunchAngleRadians = FMath::DegreesToRadians(LaunchAngleDegrees);
    FVector VerticalComponent = FVector(0.0f, 0.0f, FMath::Sin(LaunchAngleRadians)); // Vertikale Bewegung

    // Kombiniere die horizontale und vertikale Bewegung
    FVector LaunchDirection = DirectionToPlayer + VerticalComponent;

    // Normalisiere den kombinierten Vektor, um konsistente Geschwindigkeit zu gewährleisten
    LaunchDirection = LaunchDirection.GetSafeNormal();

    // Bewegung basierend auf der Geschwindigkeit und DeltaTime
    FVector Movement = LaunchDirection * speed * DeltaTime;

    // Aktualisiere die Position des Projektils
    SetActorLocation(CurrentPosition + Movement);

    // Reduziere Geschwindigkeit langsam, um einen Bogen zu simulieren (optional)
    //speed = FMath::Clamp(speed - 10.0f * DeltaTime, 100.0f, 600.0f);
   // LaunchAngleDegrees = FMath::Clamp(LaunchAngleDegrees - 24.0f * DeltaTime, -60.0f, 45.0f);
}

void AProjectile_Enemy::LaunchProjectile(float Strength, float AngleDegrees)
{
    // Convert angle from degrees to radians
    float AngleRadians = FMath::DegreesToRadians(AngleDegrees);

    // Calculate the velocity vector
    FVector LaunchDirection = FVector(FMath::Cos(AngleRadians), 0, FMath::Sin(AngleRadians));
    FVector Velocity = LaunchDirection * Strength;

    // Apply the velocity to the projectile
    ProjectileMovement->Velocity = Velocity;
    ProjectileMovement->Activate();
}

void AProjectile_Enemy::Explode()
{
   /* if (ExplosionEffect)
    {
        // Spawn Niagara-System an der Position des Projektils
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            ExplosionEffect,                     // Das Niagara-System
            GetActorLocation(),                 // Spawn-Position
            GetActorRotation(),                 // Spawn-Rotation
            FVector(1.0f),                      // Skalierung
            true,                               // Auto-Destroy
            true                                // Auto-Attach an die Welt
        );
    }*/

    if (explosion)
    {
        explosion->Activate();
    }
}

void AProjectile_Enemy::FindPlayerWithTag()
{
    FoundActors;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Player"), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        // Cast to AFH_ProjektCharacter to ensure it’s the correct type
        character = Cast<AFH_ProjektCharacter>(Actor);
    }
}

void AProjectile_Enemy::Destroy()
{
    if (explosion)
    {
        // Deaktivieren des Niagara-Systems
        explosion->Deactivate();
        Destroy();
    }
}

void AProjectile_Enemy::OverLapDmg()
{
    // Überprüfe, ob ein Charakter getroffen wurde
    if (character)
    {
        // Füge Schaden hinzu
        character->GetDmg(dmg);
    }

    // Aktiviere die Explosion, falls vorhanden
    if (explosion)
    {
        explosion->Activate();
    }

    Destroy();
}

void AProjectile_Enemy::OverLapDmgPartikel()
{
    character->GetDmg(dmg_partikel);
}

void AProjectile_Enemy::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

    AFH_ProjektCharacter* character_current = Cast<AFH_ProjektCharacter>(OtherActor);

    if(OtherActor&& OtherActor->ActorHasTag("Player"))
    {
        OverLapDmg();
    }
}

