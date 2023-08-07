// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GameplayAbility_Anim.h"
#include "AbilitySystemComponent.h"
#include "AbilityTasks/AbilityTask_PlayMontageWaitEvent.h"


UGameplayAbility_Anim::UGameplayAbility_Anim(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
}

void UGameplayAbility_Anim::ExecuteAnimTask(FAbilityTaskAnimMontageConfig AnimConfig, FGameplayTagContainer TagFilter)
{
	MontageTask = UAbilityTask_PlayMontageWaitEvent::CreatePlayMontageWaitEventProxy(this, NAME_None, TagFilter, true,
		AnimConfig.MontageToPlay, AnimConfig.Rate, AnimConfig.StartSection, AnimConfig.AnimRootMotionTranslationScale, AnimConfig.StartTimeSeconds);

	MontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnCancelled);
	MontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnInterrupted);
	MontageTask->OnBlendOut.AddDynamic(this, &ThisClass::OnBlendOut);
	MontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnCompleted);
	MontageTask->OnReceiveEvent.AddDynamic(this, &ThisClass::OnReceiveEvent);
	MontageTask->OnTimeOut.AddDynamic(this, &ThisClass::OnPlayMontageTimeOut);

	MontageTask->Activate();
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
	//auto ASC = GetAbilitySystemComponentFromActorInfo_Ensured();
	//if (Effect && ASC)
	//{
	//	auto DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(*Effect, GetAbilityLevel());
	//	AActor* Target = Cast<AActor>(EventData.Target);
	//	FGameplayAbilityTargetData_ActorArray* HitActorArrayData = new FGameplayAbilityTargetData_ActorArray();
	//	HitActorArrayData->TargetActorArray.Add(Target);
	//	FGameplayAbilityTargetDataHandle TargetData;
	//	TargetData.Add(HitActorArrayData);
	//	ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, DamageEffectSpecHandle, TargetData);
	//}
}

void UGameplayAbility_Anim::OnPlayMontageTimeOut_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	MontageTask->EndTask();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}