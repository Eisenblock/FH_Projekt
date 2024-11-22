// Fill out your copyright notice in the Description page of Project Settings.
#include "Reload_AnimNotify.h"
#include "TP_WeaponComponent.h"
#include "FH_ProjektCharacter.h"


void UReload_AnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (AFH_ProjektCharacter* character = Cast<AFH_ProjektCharacter>(MeshComp->GetOwner()))
	{
		
		UTP_WeaponComponent* weapon = character->CurrentWeaponComponent;
		weapon->GetAmmo_R();
	}
}
