// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CombatAbilitySystemComponent.h"

namespace CombatAbilitySystemComponent_Impl
{
	constexpr int32 InvalidID = 0;
	int32 IncrementingID = InvalidID;

	static int32 GetNextID()
	{
		return ++IncrementingID;
	}
}

void UCombatAbilitySystemComponent::RemoveAbility(int32 AbilityID)
{
	FBattleAbilityEntry* Ability = MappedAbilities.Find(AbilityID);
	if (Ability)
	{
		SetRemoveAbilityOnEnd(Ability->AbilitySpecHandle);
		MappedAbilities.Remove(AbilityID);
	}
}

void UCombatAbilitySystemComponent::EnableAbility(int32 AbilityID)
{
	FBattleAbilityEntry* Ability = MappedAbilities.Find(AbilityID);
	if (Ability)
	{
		Ability->AbilitySpec.InputID = AbilityID;
	}
}

void UCombatAbilitySystemComponent::DisableAbility(int32 AbilityID)
{
	FBattleAbilityEntry* Ability = MappedAbilities.Find(AbilityID);
	if (Ability)
	{
		Ability->AbilitySpec.InputID = CombatAbilitySystemComponent_Impl::InvalidID;
	}
}

int32 UCombatAbilitySystemComponent::SynchronousLoadAbility(TSoftClassPtr<UGameplayAbility> AbilityType)
{
	FGameplayAbilitySpec AbilitySpec(AbilityType.LoadSynchronous());
	int32 newID = CombatAbilitySystemComponent_Impl::GetNextID();
	AbilitySpec.InputID = newID;
	FGameplayAbilitySpecHandle AbilityHandle = GiveAbility(AbilitySpec);

	FBattleAbilityEntry* Entry = &MappedAbilities.Add(newID);
	Entry->AbilityType = AbilityType;
	Entry->AbilitySpec = AbilitySpec;
	Entry->AbilitySpecHandle = AbilityHandle;

	return newID;
}

int32 UCombatAbilitySystemComponent::FindAbilityByType(TSoftClassPtr<UGameplayAbility> AbilityType)
{
	auto MappedIterator = MappedAbilities.CreateIterator();
	while (MappedIterator)
	{
		if (MappedIterator.Value().AbilityType == AbilityType)
		{
			return MappedIterator.Key();
		}
		++MappedIterator;
	}
	return CombatAbilitySystemComponent_Impl::InvalidID;
}