// Fill out your copyright notice in the Description page of Project Settings.


#include "GameConfig/InventoryItemConfig.h"
#include "Item/ItemObject.h"
#include "Item/ItemPropertyFragment.h"
#include "DataRegistrySubsystem.h"
#include "PropertyFragment/PropertyFragment_PropBaseInfo.h"

UItemObject* UInventoryItemConfig::NewInventoryItem(FName PrefabName)
{
	UItemObject* Item = NewObject<UItemObject>();
	auto ItemPrefabRegistry = UDataRegistrySubsystem::Get()->GetRegistryForType(FName("ItemPrefabRegistry"));
	if (ItemPrefabRegistry)
	{
		auto ItemPrefab = ItemPrefabRegistry->GetCachedItem<FAncientBattlefieldItem>(FDataRegistryId(FName("ItemPrefabRegistry"), PrefabName));
		if (ItemPrefab != nullptr)
		{
			if (ItemPrefab->bPropBaseInfo)
			{
				auto PropertyFragment = NewObject<UPropertyFragment_PropBaseInfo>();
				if (ItemPrefab->PropBaseInfoPrefab.IsValid() && !ItemPrefab->PropBaseInfoPrefab.IsNone())
				{
					auto PropBaseInfoRegistry = UDataRegistrySubsystem::Get()->GetRegistryForType(FName("PropBaseInfoRegistry"));
					if (PropBaseInfoRegistry)
					{
						auto PropertyFragmentPrefab = PropBaseInfoRegistry->GetCachedItem<FPropertyFragmentPropBaseInfo>(FDataRegistryId(FName("PropBaseInfoRegistry"), ItemPrefab->PropBaseInfoPrefab));
						PropertyFragment->PropertyFragment = *PropertyFragmentPrefab;
					}
				}
				Item->AddPropertyFragment(PropertyFragment);
			}
		}
	}
	return Item;
}
