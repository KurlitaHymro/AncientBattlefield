// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/CombatAbilitySystemComponent.h"
#include "CombatCore/CombatCharacter.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/Effects/GameplayEffect_DamageDefault.h"

void UCombatAbilitySystemComponent::HandleHitEvent(FGameplayTag EventTag, AActor* Target)
{
	AActor* Instigator = GetOwner();
	auto TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);
	if (TargetASC != nullptr && IsValid(Instigator) && IsValid(Target))
	{
		FGameplayEventData Payload;
		Payload.Instigator = Instigator;
		Payload.Target = Target;
		HandleGameplayEvent(EventTag, &Payload);
		HandleDamageDefault(TargetASC);
	}
}

void UCombatAbilitySystemComponent::HandleDamageDefault(UAbilitySystemComponent* Target)
{
	if (Target)
	{
		FGameplayEffectSpecHandle Spec = MakeOutgoingSpec(UGameplayEffect_DamageDefault::StaticClass(), FGameplayEffectConstants::INVALID_LEVEL, MakeEffectContext());
		ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), Target);
	}
}
