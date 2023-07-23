// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_AbilityOperator.h"
#include "CombatCore/CombatCharacter.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"
#include "AIController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTTask_AbilityOperator)

UBTTask_AbilityOperator::UBTTask_AbilityOperator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeName = "Ability Operator";
	// instantiating to be able to use Timers
	bCreateNodeInstance = true;

	FGameplayAbilitySpec AbilitySpec(AbilityType.LoadSynchronous());
}

EBTNodeResult::Type UBTTask_AbilityOperator::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* const MyController = OwnerComp.GetAIOwner();
	MyOwnerComp = &OwnerComp;
	if (AbilityType && MyController && MyController->GetPawn())
	{
		ACombatCharacter* const MyCharacter = Cast<ACombatCharacter>(MyController->GetPawn());
		UCombatAbilitySystemComponent* const ASC = Cast<UCombatAbilitySystemComponent>(MyCharacter->GetAbilitySystemComponent());
		if (ASC)
		{
			auto AbilityID = ASC->FindAbilityByType(AbilityType);
			if (AbilityID != 0)
			{
				if (bReverse)
				{
					ASC->AbilityLocalInputReleased(AbilityID);
				}
				else
				{
					ASC->AbilityLocalInputPressed(AbilityID);
				}
				return EBTNodeResult::Succeeded;
			}
		}
	}

	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTTask_AbilityOperator::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::AbortTask(OwnerComp, NodeMemory);
}

FString UBTTask_AbilityOperator::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: %s"), bReverse ? TEXT("Released") : TEXT("Pressed"), *AbilityType.GetAssetName());
}




#if WITH_EDITOR
FName UBTTask_AbilityOperator::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Task.PlaySound.Icon");
}
#endif	// WITH_EDITOR