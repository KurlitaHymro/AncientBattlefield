// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySubsystem/Components/InventoryComponent.h"
#include "InventorySubsystem/PropertyFragment_Inventory.h"
#include "Item/ItemObject.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Items.Empty();
}

void UInventoryComponent::Add(UItemObject* Item)
{
	bool bIsAlreadyInSetPtr;
	Items.Add(Item, &bIsAlreadyInSetPtr);
	if (!bIsAlreadyInSetPtr)
	{
		InventoryAddItemDelegate.Broadcast(Item, INDEX_NONE);
		if (auto InventoryProperty = Item->FindPropertyFragment<UPropertyFragment_Inventory>())
		{
			InventoryProperty->InventoryUpdateDelegate.Broadcast(this, INDEX_NONE);
		}
	}
}

void UInventoryComponent::Remove(UItemObject* Item)
{
	if (Items.Remove(Item) > 0)
	{
		InventoryRemoveItemDelegate.Broadcast(Item, INDEX_NONE);
		if (auto InventoryProperty = Item->FindPropertyFragment<UPropertyFragment_Inventory>())
		{
			InventoryProperty->InventoryUpdateDelegate.Broadcast(nullptr, INDEX_NONE);
		}
	}
}

UItemObject* UInventoryComponent::FindItemByProperty(TSubclassOf<UItemPropertyFragment> PropertyFragmentType, FName Key)
{
	for (UItemObject* Item : Items)
	{
		if (auto PropertyFragment = Item->FindPropertyFragment(PropertyFragmentType))
		{
			if (PropertyFragment->IsKeyMatch(Key))
			{
				return Item;
			}
		}
	}
	return nullptr;
}
