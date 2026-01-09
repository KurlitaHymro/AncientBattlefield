// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_Anim.generated.h"

USTRUCT(BlueprintType)
struct FAbilityEffectCalculationConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> EffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<FGameplayTag, float> MagnitudeSeter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Stack;
};

/**
 * 
 */
UCLASS()
class ANCIENTBATTLEFIELD_API UGameplayAbility_Anim : public UGameplayAbility
{
	GENERATED_UCLASS_BODY()

public:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	void PlayMontageTask();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimAbility)
	TObjectPtr<UAnimMontage> MontageToPlay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimAbility)
	float PlayRate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimAbility)
	FName SectionName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimAbility)
	float AnimRootMotionTranslationScale = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimAbility)
	float StartTimeSeconds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimAbility)
	FGameplayTagContainer EventTagFilter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AnimAbility)
	FGameplayTagContainer InputTagFilter;

	/** GameplayEffects to apply and then remove while the animation is playing */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MontageAbility)
	TArray<TSubclassOf<UGameplayEffect>> GameplayEffectClassesWhileAnimating;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = MontageAbility)
	TMap<FGameplayTag, FAbilityEffectCalculationConfig> GameplayEffectClassesOnEvent;

protected:
	UFUNCTION(Category = Ability, BlueprintNativeEvent)
	void OnInputEvent(FGameplayEventData EventData);

	UFUNCTION(Category = Ability, BlueprintNativeEvent)
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION(Category = Ability, BlueprintNativeEvent)
	void OnInterrupted(FGameplayTag EventTag, FGameplayEventData EventData);
	
	UFUNCTION(Category = Ability, BlueprintNativeEvent)
	void OnBlendIn(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION(Category = Ability, BlueprintNativeEvent)
	void OnBlendOut(FGameplayTag EventTag, FGameplayEventData EventData);
	
	UFUNCTION(Category = Ability, BlueprintNativeEvent)
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);
	
	UFUNCTION(Category = Ability, BlueprintNativeEvent)
	void OnReceiveEvent(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION(Category = Ability, BlueprintNativeEvent)
	void OnAnimTimeOut(FGameplayTag EventTag, FGameplayEventData EventData);

protected:
	class UAbilityTask_PlayMontageWaitEvent* MontageTask;

	TArray<FActiveGameplayEffectHandle>	AppliedEffects;
};
