// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AbilityOperator.generated.h"

struct FBTAbilityOperatorTaskMemory
{

};

/**
 * 
 */
UCLASS()
class ANCIENTBATTLEFIELD_API UBTTask_AbilityOperator : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()
	
	UPROPERTY(Category = "Ability", EditAnywhere)
	TSoftObjectPtr<class UInputAction> Action;

	/** wait time in seconds */
	UPROPERTY(Category = "Ability", EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float DuringTime;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
	virtual FString GetStaticDescription() const override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif // WITH_EDITOR

protected:
	virtual void OnAbilityTimerDone();

private:
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> MyOwnerComp;

	FTimerDelegate TimerDelegate;
	FTimerHandle TimerHandle;
	TObjectPtr<class UAbilitiesInputComponent> AIC;
	TObjectPtr<class UInputAction> InputAction;
};
