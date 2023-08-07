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
	UFUNCTION(BlueprintCallable, Category = Ability)
	virtual void ExecuteAnimTask(FAbilityTaskAnimMontageConfig AnimConfig, FGameplayTagContainer EventTags);

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

	struct FActiveGameplayEffectHandle SelfEffectHandle;
};
