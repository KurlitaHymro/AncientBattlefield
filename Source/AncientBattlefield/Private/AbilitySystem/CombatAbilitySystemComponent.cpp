// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/CombatAbilitySystemComponent.h"
#include "CombatCore/CombatCharacter.h"
#include "Components/AbilitiesInputComponent.h"
#include "InputAction.h"
#include "DataRegistrySubsystem.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystem/Effects/GameplayEffect_DamageDefault.h"

UCombatAbilitySystemComponent::UCombatAbilitySystemComponent()
{
	DamageDefault.AddTag(FGameplayTag::RequestGameplayTag(FName("AncientBattlefield.Event.Hit.MeleeWeapon")));

	AdvanceInput.AdvanceInputTag = FGameplayTag::RequestGameplayTag(FName("AncientBattlefield.Status.WaitInput"));
	AdvanceInput.AbilityID = 0;
	AdvanceInput.bIsPressed = false;
}

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
		if (DamageDefault.HasTag(EventTag))
		{
			HandleDamageDefault(TargetASC);
		}
	}
}

void UCombatAbilitySystemComponent::SwitchBodyForm(FName NewBodyForm)
{
	UAbilitiesInputComponent* AIC = GetOwner()->GetComponentByClass<UAbilitiesInputComponent>();
	FName RegistryTypeName("BodyFormAbilities");
	auto Registry = UDataRegistrySubsystem::Get()->GetRegistryForType(RegistryTypeName);
	if (AIC && Registry)
	{ 
		auto NewAbilities = Registry->GetCachedItem<FBodyFormAbilities>(FDataRegistryId(RegistryTypeName, NewBodyForm));
		if (NewAbilities)
		{
			for (auto DeriveAbility : NewAbilities->DeriveAbilities)
			{
				auto AbilityID = FindAbilityByType(DeriveAbility.Value.ClassType);
				if (0 == AbilityID)
				{
					AbilityID = LoadAbilityFromType(DeriveAbility.Value.ClassType);
				}
				AIC->SetupBinding(DeriveAbility.Key.LoadSynchronous(), AbilityID);
				SwitchBodyFormDelegate.Broadcast(DeriveAbility.Key, DeriveAbility.Value);
			}
		}
	}
}

bool UCombatAbilitySystemComponent::IsWaitingAdvanceInput()
{
	return HasMatchingGameplayTag(AdvanceInput.AdvanceInputTag);
}

bool UCombatAbilitySystemComponent::IsAdvanceInputValid()
{
	return AdvanceInput.AbilityID != 0;
}

void UCombatAbilitySystemComponent::CommitAdvanceInput(int32 AbilityID, bool bPress)
{
	if (IsWaitingAdvanceInput())
	{
		if (bPress)
		{
			AdvanceInput.AbilityID = AbilityID;
			AdvanceInput.bIsPressed = true;
		}
		else
		{
			if (AdvanceInput.AbilityID == AbilityID)
			{
				AdvanceInput.bIsPressed = false;
			}
		}
	}
}

void UCombatAbilitySystemComponent::TriggerAdvanceInput()
{
	FGameplayTagContainer Container;
	Container.AddTag(AdvanceInput.AdvanceInputTag);
	RemoveActiveEffectsWithTags(Container);
	if (AdvanceInput.AbilityID)
	{
		AbilityLocalInputPressed(AdvanceInput.AbilityID);
		if (!AdvanceInput.bIsPressed)
		{
			AbilityLocalInputReleased(AdvanceInput.AbilityID);
		}
	}
	AdvanceInput.AbilityID = 0;
}

void UCombatAbilitySystemComponent::AbilityLocalInputPressed(int32 InputID)
{
	if (IsWaitingAdvanceInput())
	{
		CommitAdvanceInput(InputID, true);
	}
	else
	{
		Super::AbilityLocalInputPressed(InputID);
	}
}

void UCombatAbilitySystemComponent::AbilityLocalInputReleased(int32 InputID)
{
	if (IsWaitingAdvanceInput())
	{
		CommitAdvanceInput(InputID, false);
	}
	else
	{
		Super::AbilityLocalInputReleased(InputID);
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
