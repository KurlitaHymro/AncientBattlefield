// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/CombatAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

ACombatAIController::ACombatAIController()
{

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));
}

void ACombatAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTree)
	{
		UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComponent);
		BehaviorTreeComponent->StartTree(*BehaviorTree);
	}
	
}
