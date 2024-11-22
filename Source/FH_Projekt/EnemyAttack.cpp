// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAttack.h"
#include "FH_ProjektCharacter.h"
#include "Enemy.h"


void UEnemyAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	float dmg = 0.0f;

	if (AFH_ProjektCharacter* character = Cast<AFH_ProjektCharacter>(MeshComp->GetOwner()))
	{
		character->GetDmg(20);
	}
}
