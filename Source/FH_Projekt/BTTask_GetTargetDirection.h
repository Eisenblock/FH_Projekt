// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_GetTargetDirection.generated.h"

/**
 * 
 */
UCLASS()
class FH_PROJEKT_API UBTTask_GetTargetDirection : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_GetTargetDirection(FObjectInitializer const& object_init);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& aTreeComp, uint8* aNodeHem) override;

private:
	
	bool bHasCalculatedDirection;
};
