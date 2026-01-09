// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GameplayAbility_Anim.h"
#include "AbilitySystem/Components/CombatAbilitySystemComponent.h"
#include "Abilities/AbilityTask_PlayMontageWaitEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "CombatCore/CombatGameplayTags.h"
#include "CombatFrameworkTags.h"

UGameplayAbility_Anim::UGameplayAbility_Anim(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ClientOrServer;
	AppliedEffects.Empty();

	InputTagFilter.Reset();
	InputTagFilter.AddTag(CombatFrameworkTags::Combat_Event_Input_Triggered);
	InputTagFilter.AddTag(CombatFrameworkTags::Combat_Event_Input_Started);
	InputTagFilter.AddTag(CombatFrameworkTags::Combat_Event_Input_Ongoing);
	InputTagFilter.AddTag(CombatFrameworkTags::Combat_Event_Input_Canceled);
	InputTagFilter.AddTag(CombatFrameworkTags::Combat_Event_Input_Completed);
}

bool UGameplayAbility_Anim::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
		return MontageToPlay != nullptr && AnimInstance != nullptr && AnimInstance->GetActiveMontageInstance() == nullptr;
	}
	return false;
}

void UGameplayAbility_Anim::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		return;
	}

	UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
	if (MontageToPlay != nullptr && AnimInstance != nullptr && AnimInstance->GetActiveMontageInstance() == nullptr)
	{
		PlayMontageTask();
	}

	// 打印时间戳
	// UE_LOG(LogTemp, Log, TEXT("[%s--%ldms] ActivateAbility"), *FDateTime::Now().ToString(), (FDateTime::Now().GetTicks() / 10000) % 100000000);
}

void UGameplayAbility_Anim::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UCombatAbilitySystemComponent* CombatAbilitySystemComponent = Cast<UCombatAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo_Ensured());
	if (CombatAbilitySystemComponent)
	{
		for (auto Effect : AppliedEffects)
		{
			CombatAbilitySystemComponent->RemoveActiveGameplayEffect(Effect);
		}
	}

	// 打印时间戳
	// UE_LOG(LogTemp, Log, TEXT("[%s--%ldms] EndAbility"), *FDateTime::Now().ToString(), (FDateTime::Now().GetTicks() / 10000) % 100000000);
	 
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbility_Anim::PlayMontageTask()
{
	// Apply GameplayEffects
	UAbilitySystemComponent* AbilitySystemComponent = CurrentActorInfo->AbilitySystemComponent.Get();
	for (TSubclassOf<UGameplayEffect> EffectClass : GameplayEffectClassesWhileAnimating)
	{
		if (EffectClass)
		{
			UGameplayEffect* Effect = EffectClass->GetDefaultObject<UGameplayEffect>();
			FGameplayEffectContextHandle ContextHandle = MakeEffectContext(CurrentSpecHandle, CurrentActorInfo);
			FActiveGameplayEffectHandle EffectHandle = AbilitySystemComponent->ApplyGameplayEffectToSelf(Effect, 1.f, ContextHandle);
			if (EffectHandle.IsValid())
			{
				AppliedEffects.Add(EffectHandle);
			}
		}
	}

	MontageTask = UAbilityTask_PlayMontageWaitEvent::CreatePlayMontageWaitEventProxy(this, NAME_None, EventTagFilter, true, MontageToPlay, PlayRate, SectionName, AnimRootMotionTranslationScale, StartTimeSeconds);
	MontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnCancelled);
	MontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnInterrupted);
	MontageTask->OnBlendIn.AddDynamic(this, &ThisClass::OnBlendIn);
	MontageTask->OnBlendOut.AddDynamic(this, &ThisClass::OnBlendOut);
	MontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnCompleted);
	MontageTask->OnReceiveEvent.AddDynamic(this, &ThisClass::OnReceiveEvent);
	MontageTask->OnTimeOut.AddDynamic(this, &ThisClass::OnAnimTimeOut);
	MontageTask->ReadyForActivation();
}

void UGameplayAbility_Anim::OnInputEvent_Implementation(FGameplayEventData EventData)
{
	FGameplayTag InputEvent = EventData.EventTag;

	if (InputEvent == CombatFrameworkTags::Combat_Event_Input_Triggered)
	{

	}
	else if (InputEvent == CombatFrameworkTags::Combat_Event_Input_Started)
	{

	}
	else if (InputEvent == CombatFrameworkTags::Combat_Event_Input_Ongoing)
	{

	}
	else if (InputEvent == CombatFrameworkTags::Combat_Event_Input_Canceled)
	{

	}
	else if (InputEvent == CombatFrameworkTags::Combat_Event_Input_Completed)
	{

	}
}

void UGameplayAbility_Anim::OnCancelled_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{

}

void UGameplayAbility_Anim::OnInterrupted_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGameplayAbility_Anim::OnBlendIn_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{

}

void UGameplayAbility_Anim::OnBlendOut_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGameplayAbility_Anim::OnCompleted_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{

}

void UGameplayAbility_Anim::OnReceiveEvent_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (InputTagFilter.HasTag(EventTag))
	{
		// 如果是输入事件，直接调用 OnInputEvent
		OnInputEvent(EventData);
		return;
	}

	auto CombatAbilitySystemComponent = Cast<UCombatAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo_Ensured());
	auto EffectConfig = GameplayEffectClassesOnEvent.Find(EventTag);
	if (EffectConfig && EffectConfig->EffectClass)
	{
		FGameplayEffectContextHandle ContextHandle = CombatAbilitySystemComponent->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = CombatAbilitySystemComponent->MakeOutgoingSpec(EffectConfig->EffectClass, FGameplayEffectConstants::INVALID_LEVEL, ContextHandle);
		FGameplayEffectSpec* SpecData = SpecHandle.Data.Get();
		for (auto Seter : EffectConfig->MagnitudeSeter)
		{
			SpecData->SetSetByCallerMagnitude(Seter.Key, Seter.Value);
		}
		auto ActiveHandle = CombatAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecData);
		if (ActiveHandle.IsValid())
		{
			AppliedEffects.Add(ActiveHandle);
		}
	}
}

void UGameplayAbility_Anim::OnAnimTimeOut_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{

}
