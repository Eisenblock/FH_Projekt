// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GoDirectionPlayer.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "NavigationSystem.h"
#include "Cloud_Controller.h"
#include "Enemy_Controller.h"
#include "EnemyKeyss.h"
#include "Kismet/GameplayStatics.h"


UBTTask_GoDirectionPlayer::UBTTask_GoDirectionPlayer(FObjectInitializer const& object_init)
{
	NodeName = TEXT("GoDirection");
}

EBTNodeResult::Type UBTTask_GoDirectionPlayer::ExecuteTask(UBehaviorTreeComponent& aTreeComp, uint8* aNodeHem)
{
    auto const pAIController = Cast<ACloud_Controller>(aTreeComp.GetAIOwner());
    if (!pAIController)
    {
        UE_LOG(LogTemp, Error, TEXT("UBTTask_ChasePlayer: Failed to get AI Controller"));
        return EBTNodeResult::Failed;
    }
    //UE_LOG(LogTemp, Warning, TEXT("UBTTask_ChasePlayer: AI Controller retrieved successfully"));

    // Get Navigation System
    UNavigationSystemV1* navsys = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!navsys)
    {
        UE_LOG(LogTemp, Error, TEXT("UBTTask_ChasePlayer: Failed to get Navigation System"));
        return EBTNodeResult::Failed;
    }
    UE_LOG(LogTemp, Warning, TEXT("GoDirectionPlayer: Navigation System retrieved successfully"));

    // Get target location from the Blackboard
    FVector pLoc = pAIController->GetBB_cLOUD()->GetValueAsVector(EnemyKeyss::DirectionPlayer);
    if (pLoc.IsNearlyZero())
    {
        //UE_LOG(LogTemp, Error, TEXT("UBTTask_ChasePlayer: Target Location is invalid or zero"));
        return EBTNodeResult::Failed;
    }
    //UE_LOG(LogTemp, Warning, TEXT("UBTTask_ChasePlayer: Target Location retrieved from Blackboard: %s"), *pLoc.ToString());
    APawn* pCloud = pAIController ? pAIController->GetPawn() : nullptr;
    if (pCloud)
    {
        FVector MovementInput = pLoc; // pLoc sollte bereits ein Richtungsvektor sein
        FVector Movement = MovementInput * 5000.0f * GetWorld()->GetDeltaSeconds(); // Hier wird die Geschwindigkeit auf 0.5 skaliert
       // UE_LOG(LogTemp, Log, TEXT("New Position of Cloud Pawn: X=%.2f, Y=%.2f, Z=%.2f"), pLoc.X, pLoc.Y, pLoc.Z);
        // Berechne die neue Position, indem wir die Bewegung zur aktuellen Position hinzufügen
        FVector newPos = pCloud->GetActorLocation() + Movement;

        // Setze die neue Position des Actors
        pCloud->SetActorLocation(newPos);

        // Debug: Zeige die neue Position an
        UE_LOG(LogTemp, Log, TEXT("New Position of Cloud Pawn: X=%.2f, Y=%.2f, Z=%.2f"), newPos.X, newPos.Y, newPos.Z);
    }
    else
    {
        // Wenn pCloud null ist, gib eine Warnung aus
        UE_LOG(LogTemp, Warning, TEXT("Failed to retrieve Cloud Pawn from AIController."));
    }
  

    // Log before finishing task
    //UE_LOG(LogTemp, Warning, TEXT("UBTTask_ChasePlayer: Task Finished Successfully"));

    return EBTNodeResult::Succeeded;
	//return EBTNodeResult::Type();
}
