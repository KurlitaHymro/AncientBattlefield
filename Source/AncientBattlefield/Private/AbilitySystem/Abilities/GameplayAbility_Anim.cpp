// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GameplayAbility_Anim.h"
#include "AbilitySystemComponent.h"
#include "AbilityTasks/AbilityTask_PlayMontageWaitEvent.h"

void UGameplayAbility_Anim::PlayMontageTask(
	FGameplayTagContainer EventTags,
	bool bStopWhenAbilityEnds,
	UAnimMontage* MontageToPlay,
	FName StartSection,
	float Rate,
	float AnimRootMotionTranslationScale)
{
	MontageTask = UAbilityTask_PlayMontageWaitEvent::CreatePlayMontageWaitEventProxy(
		this, NAME_None, MontageToPlay, EventTags, Rate, StartSection, bStopWhenAbilityEnds, AnimRootMotionTranslationScale);

	MontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnCancelled);
	MontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnInterrupted);
	MontageTask->OnBlendOut.AddDynamic(this, &ThisClass::OnBlendOut);
	MontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnCompleted);
	MontageTask->OnReceiveEvent.AddDynamic(this, &ThisClass::OnReceiveEvent);
	MontageTask->Activate();
}

FGameplayAbilityTargetDataHandle UGameplayAbility_Anim::AssembleTargetData(const FGameplayEventData& EventData)
{
	FGameplayAbilityTargetDataHandle TargetData;
	AActor* Target = Cast<AActor>(EventData.Target);
	FGameplayAbilityTargetData_ActorArray* HitActorArrayData = new FGameplayAbilityTargetData_ActorArray();
	HitActorArrayData->TargetActorArray.Add(Target);
	TargetData.Add(HitActorArrayData);

	return TargetData;
}

void UGameplayAbility_Anim::OnCancelled_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	MontageTask->EndTask();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGameplayAbility_Anim::OnInterrupted_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	MontageTask->EndTask();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGameplayAbility_Anim::OnBlendOut_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	
}

void UGameplayAbility_Anim::OnCompleted_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	MontageTask->EndTask();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGameplayAbility_Anim::OnReceiveEvent_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	auto Iterator = HitEffectMap.CreateIterator();
	while (Iterator)
	{
		if (Iterator.Key() == EventTag)
		{
			break;
		}
		++Iterator;
	}

	auto ASC = GetAbilitySystemComponentFromActorInfo_Ensured();
	if (Iterator && ASC)
	{
		auto DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(Iterator.Value(), GetAbilityLevel());
		auto DataHandle = AssembleTargetData(EventData);
		ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, DamageEffectSpecHandle, DataHandle);
	}
}