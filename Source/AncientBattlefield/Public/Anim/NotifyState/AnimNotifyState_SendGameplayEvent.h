// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagAssetInterface.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AnimNotifyState_SendGameplayEvent.generated.h"

/**
 * 
 */
UCLASS()
class ANCIENTBATTLEFIELD_API UAnimNotifyState_SendGameplayEvent : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UAnimNotifyState_SendGameplayEvent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))
	FGameplayEventData BeginPayload;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))
	FGameplayEventData EndPayload;

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
