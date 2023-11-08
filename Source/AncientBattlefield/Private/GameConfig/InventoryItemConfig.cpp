// Fill out your copyright notice in the Description page of Project Settings.


#include "GameConfig/InventoryItemConfig.h"
#include "Item/ItemObject.h"
#include "Item/ItemPropertyFragment.h"
#include "DataRegistrySubsystem.h"
#include "PropertyFragment/PropertyFragment_PropBaseInfo.h"

UItemObject* UInventoryItemConfig::NewInventoryItem(UObject* Outer, FName PrefabName)
{
	UItemObject* Item = NewObject<UItemObject>(Outer);
	auto ItemPrefabRegistry = UDataRegistrySubsystem::Get()->GetRegistryForType(FName("ItemPrefabRegistry"));
	if (ItemPrefabRegistry)
	{
		auto ItemPrefab = ItemPrefabRegistry->GetCachedItem<FAncientBattlefieldItem>(FDataRegistryId(FName("ItemPrefabRegistry"), PrefabName));
		if (ItemPrefab != nullptr)
		{
			for (auto PropertyFragment : ItemPrefab->PropertyFragments)
			{
				UClass* PropertyClass = PropertyFragment.PropertyClass.LoadSynchronous();
				UItemPropertyFragment* PropertyInstance = NewObject<UItemPropertyFragment>(Item, PropertyClass);
				if (PropertyFragment.PropertyPrefab.IsValid() && !PropertyFragment.PropertyPrefab.IsNone())
				{
					PropertyInstance->InitFromRegistry(PropertyInstance->GetRegistryTypeName(), PropertyFragment.PropertyPrefab);
				}
				Item->AddPropertyFragment(PropertyInstance);
			}
		}
	}
	return Item;
}
