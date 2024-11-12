// Fill out your copyright notice in the Description page of Project Settings.


#include "rollinBall.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Ball_AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/FloatingPawnMovement.h"
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
    FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
    // Overlap-Event verbinden
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ArollinBall::OnOverlapBegin);

    // Mesh-Komponente für den Ball
    BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
    BallMesh->SetupAttachment(RootComponent);

    // Bewegungsgeschwindigkeit initialisieren
    Speed = 500.f;

    AIControllerClass = ABall_AIController::StaticClass();
}

// Called when the game starts or when spawned
void ArollinBall::BeginPlay()
{
	Super::BeginPlay();
    start = GetActorLocation();
    GetWorldTimerManager().SetTimer(TimerHandle, this, &ArollinBall::FirstDirection, starttimer, false);
    
    //directionToPlayer = GetDirection();
}

// Called every frame
void ArollinBall::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FVector NewLocation =GetActorLocation() + (directionToPlayer * 5000.0f * DeltaTime);

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
    //SetActorLocation(NewLocation);
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
    Direction.IsNormalized();
    return Direction;
}

void ArollinBall::FirstDirection()
{
    directionToPlayer = GetDirection();
}

