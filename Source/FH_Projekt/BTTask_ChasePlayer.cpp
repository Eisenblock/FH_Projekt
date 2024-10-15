// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChasePlayer.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "NavigationSystem.h"
#include "Enemy_Controller.h"
#include "EnemyKeyss.h"
#include "Kismet/GameplayStatics.h"

UBTTask_ChasePlayer::UBTTask_ChasePlayer(FObjectInitializer const& object_init)
{
    NodeName = TEXT("ChasePlayer");
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& aTreeComp, uint8* aNodeHem)
{
    // Log when the task starts executing
    //UE_LOG(LogTemp, Warning, TEXT("UBTTask_ChasePlayer: Executing Task - Chasing Player"));

    // Get AI Controller
    auto const pAIController = Cast<AEnemy_Controller>(aTreeComp.GetAIOwner());
    if (!pAIController)
    {
        //UE_LOG(LogTemp, Error, TEXT("UBTTask_ChasePlayer: Failed to get AI Controller"));
        return EBTNodeResult::Failed;
    }
    //UE_LOG(LogTemp, Warning, TEXT("UBTTask_ChasePlayer: AI Controller retrieved successfully"));

    // Get Navigation System
    UNavigationSystemV1* navsys = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!navsys)
    {
        //UE_LOG(LogTemp, Error, TEXT("UBTTask_ChasePlayer: Failed to get Navigation System"));
        return EBTNodeResult::Failed;
    }
    //UE_LOG(LogTemp, Warning, TEXT("UBTTask_ChasePlayer: Navigation System retrieved successfully"));

    // Get target location from the Blackboard
    FVector pLoc = pAIController->GetBB()->GetValueAsVector(EnemyKeyss::TargetLocation);
    if (pLoc.IsNearlyZero())
    {
        //UE_LOG(LogTemp, Error, TEXT("UBTTask_ChasePlayer: Target Location is invalid or zero"));
        return EBTNodeResult::Failed;
    }
    //UE_LOG(LogTemp, Warning, TEXT("UBTTask_ChasePlayer: Target Location retrieved from Blackboard: %s"), *pLoc.ToString());
    pAIController->MoveToLocation(pLoc);
    // Attempt to move the AI to the player's location
    if (pAIController->MoveToLocation(pLoc))
    {
        //UE_LOG(LogTemp, Warning, TEXT("UBTTask_ChasePlayer: AI moving to Target Location: %s"), *pLoc.ToString());
    }
    else
    {
        //UE_LOG(LogTemp, Error, TEXT("UBTTask_ChasePlayer: AI failed to move to Target Location"));
        return EBTNodeResult::Failed;
    }

    // Log before finishing task
    //UE_LOG(LogTemp, Warning, TEXT("UBTTask_ChasePlayer: Task Finished Successfully"));

    return EBTNodeResult::Succeeded;
}
