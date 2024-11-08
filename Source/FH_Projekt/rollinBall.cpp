// Fill out your copyright notice in the Description page of Project Settings.


#include "rollinBall.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "FH_ProjektCharacter.h"
// Sets default values
ArollinBall::ArollinBall()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
    PrimaryActorTick.bCanEverTick = true;

    // Erstellen und Einrichten der Kollisionskomponente
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->InitSphereRadius(50.0f); // Radius einstellen
    CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    RootComponent = CollisionComponent;

    // Overlap-Event verbinden
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ArollinBall::OnOverlapBegin);

    // Mesh-Komponente für den Ball
    BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
    BallMesh->SetupAttachment(RootComponent);

    // Bewegungsgeschwindigkeit initialisieren
    Speed = 500.f;
}

// Called when the game starts or when spawned
void ArollinBall::BeginPlay()
{
	Super::BeginPlay();
    start = GetActorLocation();
    directionToPlayer = GetDirection();
}

// Called every frame
void ArollinBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    // Ball bewegen - könnte auch in Richtung eines Ziels oder einer festen Richtung sein
    FVector NewLocation = GetActorLocation() + (directionToPlayer * Speed * DeltaTime);
    SetActorLocation(NewLocation);
}

// Called to bind functionality to input
void ArollinBall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ArollinBall::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        if (OtherActor->ActorHasTag("Player"))
        {
            SetActorLocation(start);
            AFH_ProjektCharacter* PlayerCharacter = Cast<AFH_ProjektCharacter>(OtherActor);
            ACharacter* character = Cast<ACharacter>(OtherActor);
           
            if (PlayerCharacter)
            {
                // Debug-Meldung in der Konsole
                UE_LOG(LogTemp, Warning, TEXT("ARollingBallPawn hat einen FH_PlayerCharacter mit dem Tag 'Player' getroffen: %s"), *OtherActor->GetName());

                // Debug-Meldung auf dem Bildschirm
                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Hit FH_PlayerCharacter with 'Player' tag: %s"), *OtherActor->GetName()));
                }

                // Beispiel: Methode von FH_PlayerCharacter aufrufen
                PlayerCharacter->GetDmg(10);
                //float ImpulseStrength = 1000.0f;  // Passe den Wert nach Bedarf an

                // Rückstoß-Impuls auf den PlayerCharacter anwenden
                PlayerCharacter->ApplyImpulse(directionToPlayer,ImpulseStrength);
            }
        }
        
        if (OtherActor->ActorHasTag("Wall"))
        {
            directionToPlayer = GetDirection();
            Speed += 5.0f;
        }
    }

}

FVector ArollinBall::GetDirection() const
{
    // Spielercharakter finden
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!PlayerCharacter)
    {
        return FVector::ZeroVector; // Wenn kein Charakter gefunden wird
    }

    // Richtung berechnen
    FVector Direction = (PlayerCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    return Direction;
}

