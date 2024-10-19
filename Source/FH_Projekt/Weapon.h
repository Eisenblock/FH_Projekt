#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundBase.h"
#include "TP_WeaponComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Weapon.generated.h"

UCLASS()
class FH_PROJEKT_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Weapon Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* weaponMesh;

	// Fire sound
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* fire_sound;

	// Empty sound
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* empty_sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* reload_sound;

	// Muzzle offset
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GamePlay")
	FVector MuzzleOffset = FVector(100.f, 0.f, 10.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GamePlay")
	UNiagaraSystem* muzzle_flash;

	// Weapon component
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GamePlay")
	UTP_WeaponComponent* weaponComponent;

	// Ammo variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GamePlay")
	int32 current_ammo = 8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GamePlay")
	int32 max_ammo = 80;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GamePlay")
	float attackSpeed;

	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
