// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerInRange.h"
#include "Enemy_Controller.h"
#include "EnemyKeyss.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

UBTService_PlayerInRange::UBTService_PlayerInRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player in Range");
}

void UBTService_PlayerInRange::OnBecomeRelevant(UBehaviorTreeComponent& treeComp, uint8* nodeName)
{
    // Korrekte Super-Aufruf
    Super::OnBecomeRelevant(treeComp, nodeName);

    // Hole den AI-Controller des Feindes
    AEnemy_Controller* aiController = Cast<AEnemy_Controller>(treeComp.GetAIOwner());
  

    APawn* enemy = aiController->GetPawn();
    

    // Hole den PlayerController und den Spieler-Pawn
    APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    

    APawn* playerPawn = playerController->GetPawn();


    // Berechne die Entfernung zwischen dem Feind und dem Spieler
    float distanceToPlayer = enemy->GetDistanceTo(playerPawn);

    // Überprüfe, ob der Spieler in Reichweite ist
    bool bIsPlayerInRange = distanceToPlayer <= range;

    // Setze den Wert im Blackboard, wenn das Blackboard vorhanden ist
    if (aiController->GetBB())
    {
        aiController->GetBB()->SetValueAsBool(EnemyKeyss::isPlayerInRange, bIsPlayerInRange);
    }
}
