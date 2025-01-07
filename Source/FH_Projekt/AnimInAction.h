// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Enemy.h"
#include "AnimInAction.generated.h"

/**
 * 
 */
UCLASS()
class FH_PROJEKT_API UAnimInAction : public UAnimNotify
{
    GENERATED_BODY()

public:
    // Override Notify function
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
