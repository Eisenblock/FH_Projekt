// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInAction.h"
#include "Enemy.h"

void UAnimInAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (!MeshComp)
    {
        return;
    }

    // Get the owning actor of the skeletal mesh
    AActor* Owner = MeshComp->GetOwner();
    if (Owner)
    {
        // Cast the owner to your Fh_ProjektCharacter class
        AEnemy* enemy = Cast<AEnemy>(Owner);
        if (enemy)
        {
            // Set the bool to true
            
            enemy->aniInAction = false;
            UE_LOG(LogTemp, Warning, TEXT("YourBoolProperty set to true by AnimNotify!"));
        }
    }
}
