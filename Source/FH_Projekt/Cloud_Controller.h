// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Cloud_Controller.generated.h"

/**
 * 
 */
UCLASS()
class FH_PROJEKT_API ACloud_Controller : public AAIController
{
	GENERATED_BODY()
	

public:
	ACloud_Controller(FObjectInitializer const& object_init);

	// Override
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* pawn) override;

	// Get Blackboard
	UFUNCTION()
	UBlackboardComponent* GetBB_cLOUD() const;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTree;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI")
	UBlackboardComponent* BlackboardComponent;
};
