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
	bCreateNodeInstance = true;
	DuringTime = 0.5f;
	bTickIntervals = true;
	FGameplayAbilitySpec AbilitySpec(AbilityType.LoadSynchronous());
	TimerDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::OnAbilityTimerDone);
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTTask_AbilityOperator::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* const MyController = OwnerComp.GetAIOwner();
	MyOwnerComp = &OwnerComp;
	if (AbilityType && MyController && MyController->GetPawn())
	{
		ACombatCharacter* const MyCharacter = Cast<ACombatCharacter>(MyController->GetPawn());
		ASC = Cast<UCombatAbilitySystemComponent>(MyCharacter->GetAbilitySystemComponent());
		if (ASC)
		{
			TimerHandle.Invalidate();
			AbilityID = ASC->FindAbilityByType(AbilityType);
			if (AbilityID != 0)
			{
				ASC->AbilityLocalInputPressed(AbilityID);
				MyController->GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, DuringTime, false);
				return EBTNodeResult::InProgress;
			}
		}
	}
	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UBTTask_AbilityOperator::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* const MyController = OwnerComp.GetAIOwner();
	if (MyController && TimerHandle.IsValid())
	{
		MyController->GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
	TimerHandle.Invalidate();

	if (ASC && AbilityID > 0)
	{
		ASC->AbilityLocalInputReleased(AbilityID);
	}

	return EBTNodeResult::Aborted;
}

void UBTTask_AbilityOperator::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);

	FBTTaskMemory* TaskMemory = GetSpecialNodeMemory<FBTTaskMemory>(NodeMemory);
	if (TaskMemory->NextTickRemainingTime)
	{
		Values.Add(FString::Printf(TEXT("remaining: %ss"), *FString::SanitizeFloat(TaskMemory->NextTickRemainingTime)));
	}
}

FString UBTTask_AbilityOperator::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s[%s]: %.1fs"), *Super::GetStaticDescription(), *AbilityType.GetAssetName(), DuringTime);
}

#if WITH_EDITOR
FName UBTTask_AbilityOperator::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Task.PlaySound.Icon");
}
#endif	// WITH_EDITOR

void UBTTask_AbilityOperator::OnAbilityTimerDone()
{
	if (ASC && AbilityID > 0)
	{
		ASC->AbilityLocalInputReleased(AbilityID);
		FinishLatentTask(*MyOwnerComp, EBTNodeResult::Succeeded);
	}
}