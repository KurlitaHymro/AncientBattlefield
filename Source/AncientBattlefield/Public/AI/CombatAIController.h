// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularAIController.h"
#include "CombatAIController.generated.h"

/**
 * 
 */
UCLASS()
class ANCIENTBATTLEFIELD_API ACombatAIController : public AModularAIController
{
	GENERATED_BODY()

public:
	ACombatAIController();

	UPROPERTY(Category = Behavior, EditAnywhere, BlueprintReadWrite)
	class UBehaviorTree* BehaviorTree;

public:
	UPROPERTY(BlueprintReadOnly)
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(BlueprintReadOnly)
	class UBlackboardComponent* BlackboardComponent;

protected:
	virtual void OnPossess(class APawn* InPawn) override;
};
