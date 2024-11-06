// Fill out your copyright notice in the Description page of Project Settings.


#include "CloudPoison.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ACloudPoison::ACloudPoison()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Erstelle und setze die CapsuleComponent als Collider
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = CapsuleComponent;  // Setze die Capsule als Root-Komponente

	// Erstelle und setze die StaticMeshComponent als Child der Root-Komponente
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ACloudPoison::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACloudPoison::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

