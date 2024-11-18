// Fill out your copyright notice in the Description page of Project Settings.


#include "GetLife.h"
#include "FH_ProjektCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h" // Include for UBoxComponent

// Sets default values
AGetLife::AGetLife()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create the Static Mesh component
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	// Create the Collision Box component
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(StaticMesh);

	// Optionally set collision properties
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	//CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AGetLife::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AGetLife::BeginPlay()
{
	Super::BeginPlay();
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AGetLife::OnOverlapBegin);
}

// Called every frame
void AGetLife::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGetLife::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor->ActorHasTag("Player"))
	{
		// Call the method to perform action
		AFH_ProjektCharacter* character = Cast<AFH_ProjektCharacter>(OtherActor);
		character->GetLife(life_amount);
		Destroy();
	}
}

void AGetLife::PerformActionOnOverlap()
{
}


