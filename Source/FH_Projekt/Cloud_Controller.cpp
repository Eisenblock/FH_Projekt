// Fill out your copyright notice in the Description page of Project Settings.


#include "Cloud_Controller.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

ACloud_Controller::ACloud_Controller(FObjectInitializer const& object_init)
{
	// Load the Behavior Tree
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> treeFinder(TEXT("/Game/FirstPerson/Blueprints/Cloud_Controll.Cloud_Controll"));
	if (treeFinder.Succeeded())
	{
		BehaviorTree = treeFinder.Object;
	}

	// Initialize Components
	BehaviorTreeComponent = object_init.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTreeComponent"));
	BlackboardComponent = object_init.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComponent"));
}

void ACloud_Controller::BeginPlay()
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

void ACloud_Controller::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	// Initialize the Blackboard
	if (BlackboardComponent && BehaviorTree)
	{
		BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	}
}

UBlackboardComponent* ACloud_Controller::GetBB_cLOUD() const
{
	return BlackboardComponent;
}
