// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	weaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon Mesh");
	RootComponent = weaponMesh;

	weaponComponent = CreateDefaultSubobject<UTP_WeaponComponent>(TEXT("WeaponComponent"));
	weaponComponent->SetupAttachment(RootComponent);
    MuzzleOffset = weaponMesh->GetSocketLocation(FName("MuzzleOffset"));

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
    Super::BeginPlay();

    if (weaponComponent != nullptr)
    {
        weaponComponent->fire_sound = fire_sound;
        weaponComponent->empty_sound = empty_sound;
        weaponComponent->MuzzleOffset = MuzzleOffset;
        weaponComponent->attackSpeed = attackSpeed;
        weaponComponent->current_ammo = current_ammo;
        weaponComponent->max_ammo = max_ammo;
        weaponComponent->reload_sound = reload_sound;
        weaponComponent->dmg = dmg;
        weaponComponent->NiagaraComponent = NiagaraComponent;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("weaponComponent is nullptr!"));
    }
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	
}

	
