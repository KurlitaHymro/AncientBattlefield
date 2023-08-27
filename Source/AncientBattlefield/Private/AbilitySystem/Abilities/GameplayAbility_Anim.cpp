// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/GameplayAbility_Anim.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"
#include "AbilityTasks/AbilityTask_PlayMontageWaitEvent.h"


UGameplayAbility_Anim::UGameplayAbility_Anim(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	AppliedEffects.Empty();
}

bool UGameplayAbility_Anim::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	if (Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
		if (MontageToPlay != nullptr && AnimInstance != nullptr && AnimInstance->GetActiveMontageInstance() == nullptr)
		{
			return true;
		}
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
		// Apply GameplayEffects
		TArray<const UGameplayEffect*> Effects;
		for (TSubclassOf<UGameplayEffect> EffectClass : GameplayEffectClassesWhileAnimating)
		{
			if (EffectClass)
			{
				Effects.Add(EffectClass->GetDefaultObject<UGameplayEffect>());
			}
		}
		if (Effects.Num() > 0)
		{
			UAbilitySystemComponent* const AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get();
			for (const UGameplayEffect* Effect : Effects)
			{
				FActiveGameplayEffectHandle EffectHandle = AbilitySystemComponent->ApplyGameplayEffectToSelf(Effect, 1.f, MakeEffectContext(Handle, ActorInfo));
				if (EffectHandle.IsValid())
				{
					AppliedEffects.Add(EffectHandle);
				}
			}
		}

		MontageTask = UAbilityTask_PlayMontageWaitEvent::CreatePlayMontageWaitEventProxy(this, NAME_None, EventTagFilter, true, MontageToPlay, PlayRate, SectionName, AnimRootMotionTranslationScale, StartTimeSeconds);
		MontageTask->OnCancelled.AddDynamic(this, &ThisClass::OnCancelled);
		MontageTask->OnInterrupted.AddDynamic(this, &ThisClass::OnInterrupted);
		MontageTask->OnBlendOut.AddDynamic(this, &ThisClass::OnBlendOut);
		MontageTask->OnCompleted.AddDynamic(this, &ThisClass::OnCompleted);
		MontageTask->OnReceiveEvent.AddDynamic(this, &ThisClass::OnReceiveEvent);
		MontageTask->OnTimeOut.AddDynamic(this, &ThisClass::OnPlayMontageTimeOut);

		MontageTask->ReadyForActivation();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

void UGameplayAbility_Anim::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	auto CombatAbilitySystemComponent = Cast<UCombatAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo_Ensured());
	if (CombatAbilitySystemComponent)
	{
		for (auto Effect : AppliedEffects)
		{
			CombatAbilitySystemComponent->RemoveActiveGameplayEffect(Effect);
		}
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbility_Anim::OnCancelled_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	
}

void UGameplayAbility_Anim::OnInterrupted_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);

	auto CombatAbilitySystemComponent = Cast<UCombatAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo_Ensured());
	CombatAbilitySystemComponent->ResetAdvanceInput();
	CombatAbilitySystemComponent->SwitchBodyForm(IdleBodyForm);
}

void UGameplayAbility_Anim::OnBlendOut_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);

	auto CombatAbilitySystemComponent = Cast<UCombatAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo_Ensured());
	if (CombatAbilitySystemComponent->IsAdvanceInputValid())
	{
		CombatAbilitySystemComponent->TriggerAdvanceInput();
	}
	else
	{
		CombatAbilitySystemComponent->ResetAdvanceInput();
	}
}

void UGameplayAbility_Anim::OnCompleted_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
	
}

void UGameplayAbility_Anim::OnReceiveEvent_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{
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

void UGameplayAbility_Anim::OnPlayMontageTimeOut_Implementation(FGameplayTag EventTag, FGameplayEventData EventData)
{

}