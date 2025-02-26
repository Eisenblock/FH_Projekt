// Fill out your copyright notice in the Description page of Project Settings.


#include "EntranceArena.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "FH_ProjektCharacter.h"

// Sets default values
AEntranceArena::AEntranceArena()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    // Create the Static Mesh Component
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetupAttachment(Mesh);
    CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    CollisionBox->SetCollisionProfileName(TEXT("BlockAll"));
    CollisionBox->SetGenerateOverlapEvents(true);
    CollisionBox->SetBoxExtent(FVector(50.f, 50.f, 50.f));
    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AEntranceArena::OnOverlapBegin);

 
}

// Called when the game starts or when spawned
void AEntranceArena::BeginPlay()
{
	Super::BeginPlay();
    TArray<AActor*> allPlayer;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Player"), allPlayer);
    playerCharacter = Cast<AFH_ProjektCharacter>(allPlayer[0]);
}

// Called every frame
void AEntranceArena::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    deltatimeH = DeltaTime;
    float Distance = FVector::Dist(GetActorLocation(), playerCharacter->GetActorLocation());
    UE_LOG(LogTemp, Warning, TEXT("Distance to Player: %f"), Distance);

    if (Distance < 1500.0f && movementStart == false && !entranceTriggered)
    {
        UE_LOG(LogTemp, Warning, TEXT("Ziel erreicht!"));
        movementStart = true;
        entranceTriggered = true;
        MoveObjectDown();
    }

    if (movementStart) {
        MovementTick();
    }
} 

void AEntranceArena::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Warning, TEXT("OnOverlapBegin triggered!"));

    if (OtherActor && OtherActor->ActorHasTag(TEXT("Player")))
    {
        UE_LOG(LogTemp, Warning, TEXT("Overlap detected with Player! Starting 3-second timer."));

        // Start a timer to change collision after 3 seconds
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEntranceArena::ChangeCollisionToBlockAll, 3.0f, false);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Overlap detected, but OtherActor does not have the 'Player' tag."));
    }
}

void AEntranceArena::ChangeCollisionToBlockAll()
{
    CollisionBox->SetCollisionProfileName(TEXT("BlockAll"));
}

void AEntranceArena::MoveObjectDown()
{
    /*
    FVector CurrentLocation = GetActorLocation();
    FVector TargetLocation = CurrentLocation - playerCharacter->GetActorLocation();
    GetWorldTimerManager().SetTimer(timeHandler, this, &AEntranceArena::MoveDownForDuration, 0.2f, true, 0.0f);*/
    GetWorldTimerManager().SetTimer(timeHandler2, this, &AEntranceArena::StopMovement, 7.0f, false);
}

void AEntranceArena::MoveDownForDuration()
{
    float Speed = 200.0f; // Geschwindigkeit pro Sekunde
    FVector CurrentLocation = GetActorLocation();
    FVector NewLocation = CurrentLocation - FVector(0.0f, 0.0f, Speed * deltatimeH);
    SetActorLocation(NewLocation);

    if (FVector::Dist(CurrentLocation, NewLocation) < 1.0f)
    {
        GetWorldTimerManager().ClearTimer(timeHandler);
    }
}

void AEntranceArena::StopMovement()
{
    movementStart = false;
    GetWorldTimerManager().ClearTimer(timeHandler);
}

void AEntranceArena::MovementTick()
{
    float Speed = 200.0f; // Geschwindigkeit pro Sekunde
    FVector CurrentLocation = GetActorLocation();
    FVector NewLocation = CurrentLocation - FVector(0.0f, 0.0f, Speed * deltatimeH);
    SetActorLocation(NewLocation);

    if (FVector::Dist(CurrentLocation, NewLocation) < 1.0f)
    {
        GetWorldTimerManager().ClearTimer(timeHandler);
    }
}

