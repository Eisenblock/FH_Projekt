// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy_Controller.generated.h"

/**
 *
 */
UCLASS()
class FH_PROJEKT_API AEnemy_Controller : public AAIController
{
	GENERATED_BODY()

public:
	AEnemy_Controller(FObjectInitializer const& object_init);

	// Override
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* pawn) override;

	// Get Blackboard
	UBlackboardComponent* GetBB() const;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI")
	UBlackboardComponent* BlackboardComponent;
};
