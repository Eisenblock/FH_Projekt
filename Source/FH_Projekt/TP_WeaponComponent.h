// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "TP_WeaponComponent.generated.h"



class AFH_ProjektCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FH_PROJEKT_API UTP_WeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	


	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* fire_sound;

	// Empty sound
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* empty_sound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* reload_sound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sound")
	UNiagaraSystem* muzzle_flash;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;


	UTP_WeaponComponent();

	bool AttachWeapon(AFH_ProjektCharacter* TargetCharacter,FName socket);
	void Fire(AFH_ProjektCharacter* TargetCharacter);
	void lostAmmo();
	void GetAmmo_R();

	float attackSpeed;
	int32 current_ammo = 8;
	int32 max_ammo = 80;
	int32 clipsize;

protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/** The Character holding this weapon*/
	AFH_ProjektCharacter* Character;


};
