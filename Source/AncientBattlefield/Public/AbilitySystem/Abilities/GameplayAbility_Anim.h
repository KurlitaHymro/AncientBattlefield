// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_Anim.generated.h"

USTRUCT(BlueprintType)
struct FAbilityTaskAnimMontageConfig
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	UAnimMontage* MontageToPlay;

	UPROPERTY(BlueprintReadWrite)
	float Rate = 1.0f;

	UPROPERTY(BlueprintReadWrite)
	FName StartSection = NAME_None;

	UPROPERTY(BlueprintReadWrite)
	float AnimRootMotionTranslationScale = 1.0f;

	UPROPERTY(BlueprintReadWrite)
	float StartTimeSeconds = 0.f;
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

	UPROPERTY(EditDefaultsOnly, Category = AnimAbility)
	TObjectPtr<UAnimMontage> MontageToPlay;

	UPROPERTY(EditDefaultsOnly, Category = AnimAbility)
	float PlayRate = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = AnimAbility)
	FName SectionName;

	UPROPERTY(EditDefaultsOnly, Category = AnimAbility)
	float AnimRootMotionTranslationScale = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = AnimAbility)
	float StartTimeSeconds;

	UPROPERTY(EditDefaultsOnly, Category = AnimAbility)
	FGameplayTagContainer EventTagFilter;

	UPROPERTY(EditDefaultsOnly, Category = AnimAbility)
	FName IdleBodyForm;

	UPROPERTY(EditDefaultsOnly, Category = AnimAbility)
	FName NextBodyForm;

	/** GameplayEffects to apply and then remove while the animation is playing */
	UPROPERTY(EditDefaultsOnly, Category = MontageAbility)
	TArray<TSubclassOf<UGameplayEffect>> GameplayEffectClassesWhileAnimating;

	UPROPERTY(EditDefaultsOnly, Category = MontageAbility)
	TMap<FGameplayTag, TSubclassOf<UGameplayEffect>> GameplayEffectClassesOnEvent;

protected:
	UFUNCTION(Category = Ability, BlueprintNativeEvent)
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION(Category = Ability, BlueprintNativeEvent)
	void OnInterrupted(FGameplayTag EventTag, FGameplayEventData EventData);
	
	UFUNCTION(Category = Ability, BlueprintNativeEvent)
	void OnBlendOut(FGameplayTag EventTag, FGameplayEventData EventData);
	
	UFUNCTION(Category = Ability, BlueprintNativeEvent)
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);
	
	UFUNCTION(Category = Ability, BlueprintNativeEvent)
	void OnReceiveEvent(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION(Category = Ability, BlueprintNativeEvent)
	void OnPlayMontageTimeOut(FGameplayTag EventTag, FGameplayEventData EventData);

protected:
	class UAbilityTask_PlayMontageWaitEvent* MontageTask;

	TArray<FActiveGameplayEffectHandle>	AppliedEffects;
};
