// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_GetTargetDirection.h"
//#include "CloudPoison.h"

#include "Cloud_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "NavigationSystem.h"
#include "Enemy_Controller.h"
#include "EnemyKeyss.h"
#include "Kismet/GameplayStatics.h"
#include "BTTask_GetTargetDirection.h"


UBTTask_GetTargetDirection::UBTTask_GetTargetDirection(FObjectInitializer const& object_init)
    : bHasCalculatedDirection(false)
{
    NodeName = TEXT("GetDirection");
}

EBTNodeResult::Type UBTTask_GetTargetDirection::ExecuteTask(UBehaviorTreeComponent& aTreeComp, uint8* aNodeHem)
{

    if (bHasCalculatedDirection)
    {
        return EBTNodeResult::Succeeded;  // Wenn bereits berechnet, Task beenden
    }
    // Get AI Controller
    ACloud_Controller* pAIController = Cast<ACloud_Controller>(aTreeComp.GetAIOwner());
    if (!pAIController)
    {
        //UE_LOG(LogTemp, Error, TEXT("UBTTask_FindPlayer: Failed to get AI Controller"));
        return EBTNodeResult::Failed;
    }
    //UE_LOG(LogTemp, Warning, TEXT("UBTTask_FindPlayer: AI Controller retrieved successfully"));

    // Get New System
    UNavigationSystemV1* pNavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!pNavSystem)
    {
        //UE_LOG(LogTemp, Error, TEXT("UBTTask_FindPlayer: Failed to get Navigation System"));
        return EBTNodeResult::Failed;
    }
    //UE_LOG(LogTemp, Warning, TEXT("UBTTask_FindPlayer: Navigation System retrieved successfully"));

    // Get Player reference
    APlayerController* pPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!pPlayerController)
    {
        //UE_LOG(LogTemp, Error, TEXT("UBTTask_FindPlayer: Failed to get Player Controller"));
        return EBTNodeResult::Failed;
    }
    //UE_LOG(LogTemp, Warning, TEXT("UBTTask_FindPlayer: Player Controller retrieved successfully"));

    APawn* pPlayer = pPlayerController ? pPlayerController->GetPawn() : nullptr;
    APawn* pCloud = pAIController ? pAIController->GetPawn() : nullptr;
    if (!pPlayer)
    {
        //UE_LOG(LogTemp, Error, TEXT("UBTTask_FindPlayer: Failed to get Player Pawn"));
        return EBTNodeResult::Failed;
    }
    //UE_LOG(LogTemp, Warning, TEXT("UBTTask_FindPlayer: Player Pawn retrieved successfully"));

  


    if (pNavSystem && pPlayer && pCloud) // Check if both the navigation system and player exist
    {
        FVector playerLocation = pPlayer->GetActorLocation();
        FVector cloudLocation = pCloud->GetActorLocation();
        //UE_LOG(LogTemp, Warning, TEXT("UBTTask_FindPlayer: Player location found: %s"), *PlayerLocation.ToString());
        
        FVector directionToPlayer = playerLocation - cloudLocation;
        directionToPlayer.Normalize();

        // Setting target location to player location
        pAIController->GetBB_cLOUD()->SetValueAsVector(EnemyKeyss::DirectionPlayer,directionToPlayer);
        //UE_LOG(LogTemp, Warning, TEXT("UBTTask_FindPlayer: Target location set in Blackboard"));
    }

    // Log before finishing task
    //UE_LOG(LogTemp, Warning, TEXT("UBTTask_FindPlayer: Task Finished Successfully"));
    bHasCalculatedDirection = true;
    // Finish execution
    FinishLatentTask(aTreeComp, EBTNodeResult::Succeeded);
    return EBTNodeResult::Succeeded;

    //return EBTNodeResult::Type();
}
