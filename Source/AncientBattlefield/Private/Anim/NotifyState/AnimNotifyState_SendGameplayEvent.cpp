// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/NotifyState/AnimNotifyState_SendGameplayEvent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

UAnimNotifyState_SendGameplayEvent::UAnimNotifyState_SendGameplayEvent()
{
	BeginPayload = FGameplayEventData();
	BeginPayload.EventTag = FGameplayTag::EmptyTag;
	EndPayload = FGameplayEventData();
	EndPayload.EventTag = FGameplayTag::EmptyTag;
}

void UAnimNotifyState_SendGameplayEvent::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	auto Instigator = MeshComp->GetOwner();
	if (Instigator)
	{
		auto AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Instigator);
		if (AbilitySystemComponent)
		{
			BeginPayload.Instigator = Instigator;
			AbilitySystemComponent->HandleGameplayEvent(BeginPayload.EventTag, &BeginPayload);
		}
	}
}

void UAnimNotifyState_SendGameplayEvent::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	auto Instigator = MeshComp->GetOwner();
	if (Instigator)
	{
		auto AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Instigator);
		if (AbilitySystemComponent)
		{
			EndPayload.Instigator = Instigator;
			AbilitySystemComponent->HandleGameplayEvent(EndPayload.EventTag, &EndPayload);
		}
	}
}
