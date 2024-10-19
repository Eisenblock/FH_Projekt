// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Enemy.h"
#include "BTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class FH_PROJEKT_API UBTTask_Attack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public :


	UBTTask_Attack(FObjectInitializer const& object_init);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& aTreeComp, uint8* aNodeHem) override;
	bool AttackMontageisFinished(AEnemy* enemy);
};
