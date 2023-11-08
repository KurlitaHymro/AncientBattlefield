// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/NotifyState/AnimNotify_SendGameplayEvent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

UAnimNotify_SendGameplayEvent::UAnimNotify_SendGameplayEvent()
{
	Payload = FGameplayEventData();
	Payload.EventTag = FGameplayTag::EmptyTag;
}

void UAnimNotify_SendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	auto Instigator = MeshComp->GetOwner();
	if (Instigator)
	{
		auto AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Instigator);
		if (AbilitySystemComponent)
		{
			Payload.Instigator = Instigator;
			AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
		}
	}
}
