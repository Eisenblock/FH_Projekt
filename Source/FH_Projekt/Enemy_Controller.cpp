#include "Enemy_Controller.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"


AEnemy_Controller::AEnemy_Controller(FObjectInitializer const& object_init)
{
	// Load the Behavior Tree
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> treeFinder(TEXT("/Game/FirstPerson/Blueprints/BT_Enemy.BT_Enemy"));
	if (treeFinder.Succeeded())
	{
		BehaviorTree = treeFinder.Object;
	}

	// Initialize Components
	BehaviorTreeComponent = object_init.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTreeComponent"));
	BlackboardComponent = object_init.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComponent"));
}

UBlackboardComponent* AEnemy_Controller::GetBB() const
{
	return BlackboardComponent;
}

void AEnemy_Controller::BeginPlay()
{
	Super::BeginPlay();

	// Start the Behavior Tree
	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
		BehaviorTreeComponent->StartTree(*BehaviorTree);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Behavior Tree is not valid!"));
	}
}

void AEnemy_Controller::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	// Initialize the Blackboard
	if (BlackboardComponent && BehaviorTree)
	{
		BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	}
}
