// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "NavigationSystem.h"
#include "Enemy_Controller.h"
#include "EnemyKeyss.h"
#include "Kismet/GameplayStatics.h"

#include "BTTask_GoDirectionPlayer.generated.h"

/**
 * 
 */
UCLASS()
class FH_PROJEKT_API UBTTask_GoDirectionPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public :

	UBTTask_GoDirectionPlayer(FObjectInitializer const& object_init);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& aTreeComp, uint8* aNodeHem) override;
	
};
