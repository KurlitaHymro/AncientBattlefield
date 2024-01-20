// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemObject.h"
#include "Item/ItemPropertyFragment.h"
#include "DataRegistrySubsystem.h"

UItemObject::UItemObject()
{
	InventoryUpdateDelegate.AddDynamic(this, &ThisClass::OnInventoryUpdate);
}

void UItemObject::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(ItemTagContainer);
}

void UItemObject::AddPropertyFragment(UItemPropertyFragment* PropertyFragmentObject, FName TemplateName)
{
	if (PropertyFragmentObject->IsDependencyReady(this))
	{
		PropertyFragmentObject->Owner = this;
		PropertyFragmentObject->Init(TemplateName);
		PropertyFragments.Add(PropertyFragmentObject);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AddPropertyFragment Lack of Dependency."));
		PropertyFragmentObject->BeginDestroy();
	}
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

FName UItemObject::RegistryType(TEXT("ItemPrefabRegistry"));
UItemObject* UItemObject::NewItemByRegistry(UObject* Outer, FName TemplateName)
{
	UItemObject* Item = NewObject<UItemObject>(Outer);
	auto ItemPrefabRegistry = UDataRegistrySubsystem::Get()->GetRegistryForType(RegistryType);
	if (ItemPrefabRegistry)
	{
		auto Template = ItemPrefabRegistry->GetCachedItem<FItemAbstract>(FDataRegistryId(RegistryType, TemplateName));
		if (Template != nullptr)
		{
			for (auto PropertyFragment : Template->PropertyFragments)
			{
				UClass* PropertyClass = PropertyFragment.PropertyClass.LoadSynchronous();
				UItemPropertyFragment* PropertyInstance = NewObject<UItemPropertyFragment>(Item, PropertyClass);
				Item->AddPropertyFragment(PropertyInstance, PropertyFragment.PropertyTemplate);
			}
		}
	}
	return Item;
}

void UItemObject::OnInventoryUpdate(UInventoryComponent* Inventory, int32 LocalID)
{
	BelongingInventory = Inventory;
	BelongingSlotID = LocalID;
}
