// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/RegisteredAbilitySystemComponent.h"

namespace RegisteredAbilitySystemComponent_Impl
{
	constexpr int32 InvalidID = 0;
	int32 IncrementingID = InvalidID;

	static int32 GetNextID()
	{
		return ++IncrementingID;
	}
}

void URegisteredAbilitySystemComponent::RemoveAbility(int32 AbilityID)
{
	FRegisteredAbilityEntry* Ability = MappedAbilities.Find(AbilityID);
	if (Ability)
	{
		SetRemoveAbilityOnEnd(Ability->AbilitySpecHandle);
		MappedAbilities.Remove(AbilityID);
	}
}

void URegisteredAbilitySystemComponent::EnableAbility(int32 AbilityID)
{
	FRegisteredAbilityEntry* Ability = MappedAbilities.Find(AbilityID);
	if (Ability)
	{
		Ability->AbilitySpec.InputID = AbilityID;
	}
}

void URegisteredAbilitySystemComponent::DisableAbility(int32 AbilityID)
{
	FRegisteredAbilityEntry* Ability = MappedAbilities.Find(AbilityID);
	if (Ability)
	{
		Ability->AbilitySpec.InputID = RegisteredAbilitySystemComponent_Impl::InvalidID;
	}
}

int32 URegisteredAbilitySystemComponent::LoadAbilityFromType(TSoftClassPtr<UGameplayAbility> AbilityType)
{
	FGameplayAbilitySpec AbilitySpec(AbilityType.LoadSynchronous());
	int32 AbilityID = RegisteredAbilitySystemComponent_Impl::GetNextID();
	AbilitySpec.InputID = AbilityID;
	FGameplayAbilitySpecHandle AbilityHandle = GiveAbility(AbilitySpec);

	FRegisteredAbilityEntry* Entry = &MappedAbilities.Add(AbilityID);
	Entry->AbilityType = AbilityType;
	Entry->AbilitySpec = AbilitySpec;
	Entry->AbilitySpecHandle = AbilityHandle;

	return AbilityID;
}

int32 URegisteredAbilitySystemComponent::FindAbilityByType(TSoftClassPtr<UGameplayAbility> AbilityType)
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
	return RegisteredAbilitySystemComponent_Impl::InvalidID;
}

TSoftClassPtr<UGameplayAbility> URegisteredAbilitySystemComponent::GetAbilityType(int32 AbilityID)
{
	FRegisteredAbilityEntry* Ability = MappedAbilities.Find(AbilityID);
	if (Ability)
	{
		return Ability->AbilityType;
	}
	return nullptr;
}
