// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimNotifyState_SelfEffectBase.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"

void UAnimNotifyState_SelfEffectBase::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MeshComp->GetOwner());
	auto EffectSpec = MakeSelfEffectSpec();
	if (EffectSpec.IsValid())
	{
		Handle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get(), AbilitySystemComponent->GetPredictionKeyForNewAction());
	}
}

void UAnimNotifyState_SelfEffectBase::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (AbilitySystemComponent != nullptr && Handle.IsValid())
	{
		AbilitySystemComponent->RemoveActiveGameplayEffect(Handle);
	}
}

FGameplayEffectSpecHandle UAnimNotifyState_SelfEffectBase::MakeSelfEffectSpec()
{
	if (AbilitySystemComponent != nullptr && SelfEffect != nullptr)
	{
		FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(SelfEffect, 0, ContextHandle);
		FGameplayEffectSpec* Spec = SpecHandle.Data.Get();
		for (auto Seter : MagnitudeSeter)
		{
			Spec->SetSetByCallerMagnitude(Seter.Key, Seter.Value);
		}
		return SpecHandle;
	}
	return FGameplayEffectSpecHandle();
}

