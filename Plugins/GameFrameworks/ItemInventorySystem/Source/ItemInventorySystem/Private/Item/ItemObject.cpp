// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemObject.h"
#include "Item/ItemPropertyFragment.h"

void UItemObject::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(ItemTagContainer);
}


UItemPropertyFragment* UItemObject::FindPropertyFragment(TSubclassOf<class UItemPropertyFragment> PropertyFragmentType)
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

void UItemObject::AddPropertyFragment(TSubclassOf<UItemPropertyFragment> PropertyFragmentType, UDataTable* DataTable, FString PropertyFragmentPrefabName)
{
	auto PropertyFragment = PropertyFragmentType.GetDefaultObject();
	if (DataTable != nullptr && !PropertyFragmentPrefabName.IsEmpty())
	{
		PropertyFragment->InitFromMetaDataTable(DataTable, PropertyFragmentPrefabName);
	}
	PropertyFragments.Add(PropertyFragment);
}
