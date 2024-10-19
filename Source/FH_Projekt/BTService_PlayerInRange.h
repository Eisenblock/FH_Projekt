// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_PlayerInRange.generated.h"

/**
 * 
 */
UCLASS()
class FH_PROJEKT_API UBTService_PlayerInRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public :

	UBTService_PlayerInRange();

	void OnBecomeRelevant(UBehaviorTreeComponent& treeComp, uint8* nodeName);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float range = 100.0f;
};
