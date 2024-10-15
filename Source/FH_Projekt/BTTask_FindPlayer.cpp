#include "BTTask_FindPlayer.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "NavigationSystem.h"
#include "Enemy_Controller.h"
#include "EnemyKeyss.h"
#include "Kismet/GameplayStatics.h"


UBTTask_FindPlayer::UBTTask_FindPlayer(FObjectInitializer const& object_init)
{
    NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UBTTask_FindPlayer::ExecuteTask(UBehaviorTreeComponent& aTreeComp, uint8* aNodeHem)
{
    NodeName = TEXT("Find Player Location");

    // Log when the task starts executing
    //UE_LOG(LogTemp, Warning, TEXT("UBTTask_FindPlayer: Executing Task - Finding Player Location"));

    // Get AI Controller
    AEnemy_Controller* pAIController = Cast<AEnemy_Controller>(aTreeComp.GetAIOwner());
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
    if (!pPlayer)
    {
        //UE_LOG(LogTemp, Error, TEXT("UBTTask_FindPlayer: Failed to get Player Pawn"));
        return EBTNodeResult::Failed;
    }
    //UE_LOG(LogTemp, Warning, TEXT("UBTTask_FindPlayer: Player Pawn retrieved successfully"));

    if (pNavSystem && pPlayer) // Check if both the navigation system and player exist
    {
        FVector PlayerLocation = pPlayer->GetActorLocation();
        //UE_LOG(LogTemp, Warning, TEXT("UBTTask_FindPlayer: Player location found: %s"), *PlayerLocation.ToString());

        // Setting target location to player location
        pAIController->GetBB()->SetValueAsVector(EnemyKeyss::TargetLocation, PlayerLocation);
        //UE_LOG(LogTemp, Warning, TEXT("UBTTask_FindPlayer: Target location set in Blackboard"));
    }

    // Log before finishing task
    //UE_LOG(LogTemp, Warning, TEXT("UBTTask_FindPlayer: Task Finished Successfully"));

    // Finish execution
    FinishLatentTask(aTreeComp, EBTNodeResult::Succeeded);
    return EBTNodeResult::Succeeded;
}