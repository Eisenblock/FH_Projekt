// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	weaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon Mesh");
	RootComponent = weaponMesh;

	weaponComponent = CreateDefaultSubobject<UTP_WeaponComponent>(TEXT("WeaponComponent"));
	weaponComponent->SetupAttachment(RootComponent);
	current_ammo = 8;
	max_ammo = 80;

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	weaponComponent->fire_sound = fire_sound;
	weaponComponent->empty_sound = empty_sound;
	//weaponComponent->muzzle_flash = muzzle_flash;
	weaponComponent->MuzzleOffset = MuzzleOffset;
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

