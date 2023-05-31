// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_Anim.generated.h"

/**
 * 
 */
UCLASS()
class ANCIENTBATTLEFIELD_API UGameplayAbility_Anim : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, TSubclassOf<UGameplayEffect>> HitEffectMap;

protected:
	UFUNCTION(BlueprintCallable, Category = Ability)
	virtual void PlayMontageTask(
		FGameplayTagContainer EventTags,
		bool bStopWhenAbilityEnds,
		UAnimMontage* MontageToPlay,
		FName StartSection,
		float Rate = 1.0f,
		float AnimRootMotionTranslationScale = 1.0f);

	UFUNCTION(BlueprintCallable, Category = Ability)
	virtual FGameplayAbilityTargetDataHandle AssembleTargetData(const FGameplayEventData& EventData);

	UFUNCTION(Category = Ability, BlueprintNativeEvent)
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);
	virtual void OnCancelled_Implementation(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION(Category = Ability, BlueprintNativeEvent)
	void OnInterrupted(FGameplayTag EventTag, FGameplayEventData EventData);
	virtual void OnInterrupted_Implementation(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION(Category = Ability, BlueprintNativeEvent)
	void OnBlendOut(FGameplayTag EventTag, FGameplayEventData EventData);
	virtual void OnBlendOut_Implementation(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION(Category = Ability, BlueprintNativeEvent)
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);
	virtual void OnCompleted_Implementation(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION(Category = Ability, BlueprintNativeEvent)
	void OnReceiveEvent(FGameplayTag EventTag, FGameplayEventData EventData);
	virtual void OnReceiveEvent_Implementation(FGameplayTag EventTag, FGameplayEventData EventData);

protected:
	class UAbilityTask_PlayMontageWaitEvent* MontageTask;

	struct FActiveGameplayEffectHandle SelfEffectHandle;
};
