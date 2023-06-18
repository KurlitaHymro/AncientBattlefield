// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemObject.h"
#include "Item/ItemPropertyFragment.h"

void UItemObject::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(ItemTagContainer);
}


UItemPropertyFragment* UItemObject::FindPropertyFragment(TSubclassOf<UItemPropertyFragment> PropertyFragmentType)
{
	if (PropertyFragmentType)
	{
		for (UItemPropertyFragment* Fragment : PropertyFragments)
		{
			if (Fragment->IsA(PropertyFragmentType))
			{
				return Fragment;
			}
		}
	}
	return nullptr;
}

UItemPropertyFragment* UItemObject::ConstructPropertyFragment(TSubclassOf<UItemPropertyFragment> PropertyFragmentType)
{
	auto PropertyFragment = PropertyFragmentType.GetDefaultObject();
	PropertyFragment->Instantiate(this);
	PropertyFragments.Add(PropertyFragment);

	return PropertyFragment;
}
