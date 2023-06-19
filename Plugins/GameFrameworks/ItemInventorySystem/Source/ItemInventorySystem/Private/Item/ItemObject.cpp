// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemObject.h"
#include "Item/ItemPropertyFragment.h"

void UItemObject::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(ItemTagContainer);
}

void UItemObject::AddPropertyFragment(UItemPropertyFragment* PropertyFragmentObject)
{
	PropertyFragmentObject->Instantiate(this);
	PropertyFragments.Add(PropertyFragmentObject);
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