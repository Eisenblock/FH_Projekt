// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EnemyAttack.generated.h"

/**
 * 
 */
UCLASS()
class FH_PROJEKT_API UEnemyAttack : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	FName NotifyName;

	// Hier ist der Punkt, an dem wir die Methode aufrufen
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
