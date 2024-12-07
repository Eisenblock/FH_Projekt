// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAttack.h"
#include "FH_ProjektCharacter.h"
#include "Enemy.h"


void UEnemyAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (AEnemy* Enemy = Cast<AEnemy>(MeshComp->GetOwner()))
    {
        if (AFH_ProjektCharacter* Character = Cast<AFH_ProjektCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()))
        {
            // Schaden zufügen
            Character->GetDmg(20);
        }
    }
}
