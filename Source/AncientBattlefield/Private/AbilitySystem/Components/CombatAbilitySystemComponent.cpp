// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Components/CombatAbilitySystemComponent.h"
#include "CombatCore/CombatCharacter.h"
#include "CombatCore/CombatGameplayTags.h"
#include "CombatFrameworkTags.h"
#include "Components/AbilitiesInputComponent.h"
#include "InputAction.h"
#include "DataRegistrySubsystem.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/Effects/GameplayEffect_DamageDefault.h"

UCombatAbilitySystemComponent::UCombatAbilitySystemComponent()
{
	DamageDefault.Reset();
	DamageDefault.AddTag(CombatGameplayTags::Combat_AbilitySystem_Event_MeleeWeapon);
}

void UCombatAbilitySystemComponent::HandleHitEvent(FGameplayTag EventTag, AActor* TargetActor)
{
	AActor* Instigator = GetOwner();
	auto TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(TargetActor);
	if (TargetASC != nullptr && IsValid(Instigator) && IsValid(TargetActor))
	{
		FGameplayEventData Payload;
		Payload.Instigator = Instigator;
		Payload.Target = TargetActor;
		HandleGameplayEvent(EventTag, &Payload);
		if (DamageDefault.HasTag(EventTag))
		{
			HandleDamageDefault(TargetASC);
		}
	}
}

void UCombatAbilitySystemComponent::HandleDamageDefault(UAbilitySystemComponent* TargetASC)
{
	if (TargetASC)
	{
		FGameplayEffectSpecHandle Spec = MakeOutgoingSpec(UGameplayEffect_DamageDefault::StaticClass(), FGameplayEffectConstants::INVALID_LEVEL, MakeEffectContext());
		ApplyGameplayEffectSpecToTarget(*Spec.Data.Get(), TargetASC);
	}
}
