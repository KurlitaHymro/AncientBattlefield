// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayEffect.h"
#include "AnimNotifyState_SelfEffectBase.generated.h"

/**
 * 
 */
UCLASS()
class COMBATFRAMEWORK_API UAnimNotifyState_SelfEffectBase : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Ability")
	TSubclassOf<UGameplayEffect> SelfEffect;

	UPROPERTY(EditAnywhere, Category = "Ability")
	TMap<FGameplayTag, float> MagnitudeSeter;

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	virtual FGameplayEffectSpecHandle MakeSelfEffectSpec();

protected:
	UAbilitySystemComponent* AbilitySystemComponent;

	FActiveGameplayEffectHandle Handle;
};
