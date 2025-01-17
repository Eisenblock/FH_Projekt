// Fill out your copyright notice in the Description page of Project Settings.


#include "MuzzelFlash.h"
#include "TP_WeaponComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
AMuzzelFlash::AMuzzelFlash()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MuzzleFlashMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MuzzleFlashMesh"));
	RootComponent = MuzzleFlashMesh;
}

// Called when the game starts or when spawned
void AMuzzelFlash::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle timehandler;
	GetWorld()->GetTimerManager().SetTimer(timehandler, this, &AMuzzelFlash::KillObject, 0.07f, false);
    WeaponComponent = Cast<UTP_WeaponComponent>(GetComponentByClass(UTP_WeaponComponent::StaticClass()));
}


// Called every frame
void AMuzzelFlash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//FVector pos = WeaponComponent->GetSocketPos();
	//SetActorLocation(pos);

}

void AMuzzelFlash::KillObject()
{
	Destroy();
}

void AMuzzelFlash::GetWeaponComponent()
{

}

