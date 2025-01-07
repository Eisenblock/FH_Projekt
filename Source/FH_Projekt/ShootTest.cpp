// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootTest.h"
#include "FH_ProjektCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AShootTest::AShootTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Static Mesh erstellen und als Root setzen
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    //Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    RootComponent = Mesh;

    // Sphere Collider erstellen
    SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
    SphereCollider->SetupAttachment(Mesh); // Collider an Mesh anhängen
    SphereCollider->SetSphereRadius(100.0f); // Radius des Sphären-Colliders
    SphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    //SphereCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
    //SphereCollider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Spieler-Charaktere reagieren auf Overlap
    SphereCollider->SetGenerateOverlapEvents(true);

    SphereCollider2 = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider2"));
    SphereCollider2->SetupAttachment(Mesh); // Collider an Mesh anhängen
    SphereCollider2->SetSphereRadius(100.0f); // Radius des Sphären-Colliders
    SphereCollider2->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    //SphereCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
    //SphereCollider->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Spieler-Charaktere reagieren auf Overlap
    SphereCollider2->SetGenerateOverlapEvents(true);

    // Niagara-Komponente erstellen
    explosion = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffect"));
    explosion->SetupAttachment(Mesh); // Niagara-Effekt an Mesh anhängen
    explosion->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    explosion->SetGenerateOverlapEvents(true);

    // Overlap-Event binden
    SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &AShootTest::OnSphereOverlap);
    SphereCollider2->OnComponentBeginOverlap.AddDynamic(this, &AShootTest::OnSphereOverlap2);
    bHit = false;
}

// Called when the game starts or when spawned
void AShootTest::BeginPlay()
{
	Super::BeginPlay();
    
    SphereCollider2->OnComponentBeginOverlap.AddDynamic(this, &AShootTest::OnSphereOverlap2);
    SphereCollider2->SetHiddenInGame(true);

    SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &AShootTest::OnSphereOverlap);
    

   // GetWorld()->GetTimerManager().SetTimer(SpeedZeroTimerHandle, this, &AShootTest::Explode, explode_time, false);

    FindPlayerWithTag();

   

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
void AShootTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    // Aktuelle Position des Projektils
    FVector CurrentPosition = GetActorLocation();

   // SphereCollider->SetRelativeLocation(CurrentPosition);

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
    //LaunchAngleDegrees = FMath::Clamp(LaunchAngleDegrees - 24.0f * DeltaTime, -60.0f, 45.0f);
}

void AShootTest::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag("Player") && !bHit)
    {
        AFH_ProjektCharacter* character2 = Cast<AFH_ProjektCharacter>(OtherActor);
        character2->GetDmg(20);

        if (explosion)
        {
            explosion->Activate();
        }

        bHit = true;
        Mesh->SetHiddenInGame(true);
        SphereCollider->SetHiddenInGame(true);
        UE_LOG(LogTemp, Warning, TEXT("SphereOverlapActor: Overlap with Player detected: %s"), *OtherActor->GetName());
        GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AShootTest::KillObject, 3.0f, false);
    }

    if (OtherActor && OtherActor->ActorHasTag("Ground") && !bHit)
    {
        if (explosion)
        {
            explosion->Activate();
        }

        speed = 0;
        //bHit = true;
        Mesh->SetHiddenInGame(true);
        SphereCollider->SetHiddenInGame(true);
        SphereCollider2->SetHiddenInGame(false);
        UE_LOG(LogTemp, Warning, TEXT("SphereOverlapActor: Overlap with Player detected: %s"), *OtherActor->GetName());
        GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AShootTest::KillObject, 0.5f, false);
    }
}

void AShootTest::OnSphereOverlap2(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag("Player") && !bHit)
    {
        AFH_ProjektCharacter* character2 = Cast<AFH_ProjektCharacter>(OtherActor);
        character2->GetDmg(20);

        if (explosion)
        {
            explosion->Activate();
        }

        bHit = true;
        Mesh->SetHiddenInGame(true);
        SphereCollider->SetHiddenInGame(true);
        UE_LOG(LogTemp, Warning, TEXT("SphereOverlapActor: Overlap with Player detected: %s"), *OtherActor->GetName());
        GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &AShootTest::KillObject, 3.0f, false);
    }
}

/*void AShootTest::OnPartikelSOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag("Player"))
    {
        AFH_ProjektCharacter* character2 = Cast<AFH_ProjektCharacter>(OtherActor);
        character2->GetDmg(5);


        UE_LOG(LogTemp, Warning, TEXT("SphereOverlapActor: Overlap with Player detected: %s"), *OtherActor->GetName());
    }
}*/

void AShootTest::FindPlayerWithTag()
{
    FoundActors;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Player"), FoundActors);

    for (AActor* Actor : FoundActors)
    {
        // Cast to AFH_ProjektCharacter to ensure it’s the correct type
        character = Cast<AFH_ProjektCharacter>(Actor);
    }
}

void AShootTest::Explode()
{
    explosion->Activate();
}

void AShootTest::KillObject()
{
    Destroy();
}

