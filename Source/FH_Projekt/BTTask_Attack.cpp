// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Attack.h"
#include "Enemy.h"
#include "Enemy_Controller.h"
#include "EnemyKeyss.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Animation/AnimInstance.h"

// Definieren einer Log-Kategorie für das Debugging
DEFINE_LOG_CATEGORY_STATIC(LogBTTaskAttack, Log, All);

UBTTask_Attack::UBTTask_Attack(FObjectInitializer const& object_init)
{
    NodeName = "Attack Task";
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& aTreeComp, uint8* aNodeHem)
{
    // Hole den AI-Controller
    AEnemy_Controller* aiController = Cast<AEnemy_Controller>(aTreeComp.GetAIOwner());
    if (aiController == nullptr)
    {
        UE_LOG(LogBTTaskAttack, Warning, TEXT("AI Controller is null"));
        return EBTNodeResult::Failed;
    }

    // Hole den Gegner (Pawn)
    AEnemy* enemy = Cast<AEnemy>(aiController->GetPawn());
    if (enemy == nullptr)
    {
        UE_LOG(LogBTTaskAttack, Warning, TEXT("Enemy Pawn is null"));
        return EBTNodeResult::Failed;
    }

    // Debug-Log für den Angriffsmontage-Status
    bool bIsMontageFinished = AttackMontageisFinished(enemy);
    UE_LOG(LogBTTaskAttack, Log, TEXT("Attack Montage Finished: %s"), bIsMontageFinished ? TEXT("Yes") : TEXT("No"));

    if (bIsMontageFinished)
    {
        // Hole den Wert aus dem Blackboard (ob Spieler in Reichweite ist)
        bool canAttack = aiController->GetBB()->GetValueAsBool(EnemyKeyss::isPlayerInRange);
        UE_LOG(LogBTTaskAttack, Log, TEXT("Can Attack: %s"), canAttack ? TEXT("Yes") : TEXT("No"));

        if (canAttack)
        {
            UE_LOG(LogBTTaskAttack, Log, TEXT("Enemy is attacking!"));
            enemy->Attack();  // Führe den Angriff aus
        }
        else
        {
            UE_LOG(LogBTTaskAttack, Log, TEXT("Enemy cannot attack because the player is not in range."));
        }
    }

    return EBTNodeResult::Succeeded;
}

bool UBTTask_Attack::AttackMontageisFinished(AEnemy* enemy)
{
    if (enemy == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Enemy is null"));
        return false;
    }

    // Überprüfe, ob das Mesh existiert
    USkeletalMeshComponent* EnemyMesh = enemy->GetMesh();
    if (EnemyMesh == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Skeletal Mesh is null"));
        return false;
    }

    // Überprüfe, ob die AnimInstance existiert
    UAnimInstance* animInstance = EnemyMesh->GetAnimInstance();
    if (animInstance == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("AnimInstance is null"));
        return false;
    }

    // Überprüfe, ob die AnimMontage existiert
    if (enemy->attack_anim == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Attack animation montage is null"));
        return false;
    }

    // Überprüfe, ob die Montage gestoppt ist
    bool bIsStopped = animInstance->Montage_GetIsStopped(enemy->attack_anim);
    UE_LOG(LogTemp, Log, TEXT("Montage is stopped: %s"), bIsStopped ? TEXT("Yes") : TEXT("No"));
    return bIsStopped;
}
