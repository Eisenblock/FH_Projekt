// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootEnemy.h"
#include "FH_ProjektCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include <Kismet/GameplayStatics.h>


// Sets default values
AShootEnemy::AShootEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(50.0f); // Radius einstellen
    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    RootComponent = CollisionComponent;
    FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
    // Overlap-Event verbinden
   // CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ArollinBall::OnOverlapBegin);

    // Mesh-Komponente für den Ball
    BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
    BallMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AShootEnemy::BeginPlay()
{
	Super::BeginPlay();
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Player"), FoundActors);

    if (FoundActors.Num() > 0) // Wenn ein Actor mit dem Tag gefunden wurde
    {
        AActor* FoundActor = FoundActors[0]; // Nimm den ersten gefundenen Actor

        // Versuche, ihn in den gewünschten Typ zu casten (z. B. zu APlayerCharacter)
        character = Cast<AFH_ProjektCharacter>(FoundActor);

        start_Pos = (character->GetActorLocation() - GetActorLocation());
        start_Pos.Normalize();

    }
}

// Called every frame
void AShootEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FVector Movement = start_Pos * speed * DeltaTime;

    FloatingPawnMovement->AddInputVector(start_Pos * speed);

    // Aktualisiere die Position des Actors
   // SetActorLocation(GetActorLocation() + Movement);
}

// Called to bind functionality to input
void AShootEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

