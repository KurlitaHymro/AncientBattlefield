// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySubsystem/Abilities/GameplayAbility_AnimMove.h"
#include "AbilitySystemComponent.h"
#include "AbilityTasks/AbilityTask_PlayMontageWaitEvent.h"

void UGameplayAbility_AnimMove::PlayMontageTask(
	FGameplayTagContainer EventTags,
	bool bStopWhenAbilityEnds,
	UAnimMontage* MontageToPlay,
	FName StartSection,
	float Rate,
	float AnimRootMotionTranslationScale)
{
	MontageTask = UAbilityTask_PlayMontageWaitEvent::CreatePlayMontageWaitEventProxy(
		this, NAME_None, MontageToPlay, EventTags, Rate, StartSection, bStopWhenAbilityEnds, AnimRootMotionTranslationScale);

	MontageTask->OnCancelled.AddDynamic(this, &UGameplayAbility_AnimMove::OnCancelled);
	MontageTask->OnInterrupted.AddDynamic(this, &UGameplayAbility_AnimMove::OnInterrupted);
	MontageTask->OnBlendOut.AddDynamic(this, &UGameplayAbility_AnimMove::OnBlendOut);
	MontageTask->OnCompleted.AddDynamic(this, &UGameplayAbility_AnimMove::OnCompleted);
	MontageTask->OnReceiveEvent.AddDynamic(this, &UGameplayAbility_AnimMove::OnReceiveEvent);
	MontageTask->Activate();
}

FGameplayAbilityTargetDataHandle UGameplayAbility_AnimMove::AssembleTargetData(const FGameplayEventData& EventData)
{
	FGameplayAbilityTargetDataHandle TargetData;
	AActor* Target = Cast<AActor>(EventData.Target);
	FGameplayAbilityTargetData_ActorArray* HitActorArrayData = new FGameplayAbilityTargetData_ActorArray();
	HitActorArrayData->TargetActorArray.Add(Target);
	TargetData.Add(HitActorArrayData);

	return TargetData;
}

void UGameplayAbility_AnimMove::OnCancelled_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	MontageTask->EndTask();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGameplayAbility_AnimMove::OnInterrupted_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	MontageTask->EndTask();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGameplayAbility_AnimMove::OnBlendOut_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	
}

void UGameplayAbility_AnimMove::OnCompleted_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	MontageTask->EndTask();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGameplayAbility_AnimMove::OnReceiveEvent_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
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