// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemObject.h"
#include "Item/ItemPropertyFragment.h"
#include "DataRegistrySubsystem.h"

void UItemObject::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(ItemTagContainer);
}

void UItemObject::AddPropertyFragment(UItemPropertyFragment* PropertyFragmentObject)
{
	PropertyFragmentObject->Owner = this;
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

UItemObject* UItemObject::NewItemByRegistry(UObject* Outer, FName PrefabName)
{
	UItemObject* Item = NewObject<UItemObject>(Outer);
	auto ItemPrefabRegistry = UDataRegistrySubsystem::Get()->GetRegistryForType(FName("ItemPrefabRegistry"));
	if (ItemPrefabRegistry)
	{
		auto ItemPrefab = ItemPrefabRegistry->GetCachedItem<FItemAbstract>(FDataRegistryId(FName("ItemPrefabRegistry"), PrefabName));
		if (ItemPrefab != nullptr)
		{
			for (auto PropertyFragment : ItemPrefab->PropertyFragments)
			{
				UClass* PropertyClass = PropertyFragment.PropertyClass.LoadSynchronous();
				UItemPropertyFragment* PropertyInstance = NewObject<UItemPropertyFragment>(Item, PropertyClass);
				PropertyInstance->Init(Item, PropertyFragment.PropertyPrefab, PropertyInstance->GetRegistryTypeName(), nullptr);
			}
		}
	}
	return Item;
}
