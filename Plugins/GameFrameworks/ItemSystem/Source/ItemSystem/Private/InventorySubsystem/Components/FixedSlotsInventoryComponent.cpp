// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySubsystem/Components/FixedSlotsInventoryComponent.h"
#include "InventorySubsystem/PropertyFragment_Inventory.h"
#include "Item/ItemObject.h"
#include "ItemSystem.h"

// Sets default values for this component's properties
UFixedSlotsInventoryComponent::UFixedSlotsInventoryComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FixedSlots.Empty();
	SlotCount = 0;
}

void UFixedSlotsInventoryComponent::PostLoad()
{
	Super::PostLoad();

	if (SlotCount > FixedSlots.Num())
	{
		FFixedInventorySlot SlotTemplate;
		SlotTemplate.ItemRequiredTags.Reset();
		SlotTemplate.ItemBlockedTags.Reset();
		SlotTemplate.ItemObject = nullptr;
		for (int32 AddSlotIndex = FixedSlots.Num(); AddSlotIndex < SlotCount; ++AddSlotIndex)
		{
			FixedSlots.Emplace(SlotTemplate);
		}
	}
	else if (SlotCount < FixedSlots.Num())
	{
		SlotCount = FixedSlots.Num();
	}
}

void UFixedSlotsInventoryComponent::Add(UItemObject* Item)
{
	int32 FoundSlotIndex = FindAvailableSlot(Item);
	if (FoundSlotIndex > INDEX_NONE)
	{
		AddTo(FoundSlotIndex, Item);
	}
}

void UFixedSlotsInventoryComponent::Remove(UItemObject* Item)
{
	int32 FoundSlotIndex = FixedSlots.IndexOfByPredicate([Item](const FFixedInventorySlot& Slot) { return Slot.ItemObject == Item; });
	if (FoundSlotIndex != INDEX_NONE)
	{
		RemoveFrom(FoundSlotIndex);
	}
}

UItemObject* UFixedSlotsInventoryComponent::FindItemByProperty(TSubclassOf<UItemPropertyFragment> PropertyFragmentType, FName Key)
{
	for (FFixedInventorySlot& Slot : FixedSlots)
	{
		if (auto Item = Slot.ItemObject)
		{
			if (auto PropertyFragment = Item->FindPropertyFragment(PropertyFragmentType))
			{
				if (PropertyFragment->IsKeyMatch(Key))
				{
					return Item;
				}
			}
		}
	}
	return nullptr;
}

bool UFixedSlotsInventoryComponent::CanAddTo(int32 SlotIndex, UItemObject* Item)
{
	return FixedSlots.IsValidIndex(SlotIndex) && !FixedSlots[SlotIndex].ItemObject && CanAccommodate(SlotIndex, Item);
}

void UFixedSlotsInventoryComponent::AddTo(int32 SlotIndex, UItemObject* Item)
{
	if (CanAddTo(SlotIndex, Item))
	{
		FixedSlots[SlotIndex].ItemObject = Item;
		InventoryAddItemDelegate.Broadcast(Item, SlotIndex);
		if (auto InventoryProperty = Item->FindPropertyFragment<UPropertyFragment_Inventory>())
		{
			InventoryProperty->InventoryUpdateDelegate.Broadcast(this, SlotIndex);
		}
	}
}

void UFixedSlotsInventoryComponent::RemoveFrom(int32 SlotIndex)
{
	if (FixedSlots.IsValidIndex(SlotIndex) && FixedSlots[SlotIndex].ItemObject)
	{
		UItemObject* Item = FixedSlots[SlotIndex].ItemObject;
		InventoryRemoveItemDelegate.Broadcast(Item, SlotIndex);
		if (auto InventoryProperty = Item->FindPropertyFragment<UPropertyFragment_Inventory>())
		{
			InventoryProperty->InventoryUpdateDelegate.Broadcast(nullptr, INDEX_NONE);
		}
		FixedSlots[SlotIndex].ItemObject = nullptr;
	}
}

UItemObject* UFixedSlotsInventoryComponent::Get(int32 SlotIndex)
{
	return FixedSlots.IsValidIndex(SlotIndex) ? FixedSlots[SlotIndex].ItemObject : nullptr;
}

int32 UFixedSlotsInventoryComponent::FindAvailableSlot(UItemObject* Item)
{
	for (int32 Index = 0; Index < SlotCount; Index++)
	{
		if (!FixedSlots[Index].ItemObject && CanAccommodate(Index, Item))
		{
			return Index;
		}
	}
	return INDEX_NONE;
}

bool UFixedSlotsInventoryComponent::CanAccommodate(int32 SlotIndex, UItemObject* Item)
{
	if (FixedSlots.IsValidIndex(SlotIndex))
	{
		FFixedInventorySlot& Slot = FixedSlots[SlotIndex];
		return Item->ItemTagContainer.HasAll(Slot.ItemRequiredTags) && !Item->ItemTagContainer.HasAny(Slot.ItemBlockedTags);
	}
	return false;
}

void UFixedSlotsInventoryComponent::AddSlot(FGameplayTagContainer ItemRequiredTags, FGameplayTagContainer ItemBlockedTags)
{
	FFixedInventorySlot SlotTemplate;
	SlotTemplate.ItemRequiredTags.AppendTags(ItemRequiredTags);
	SlotTemplate.ItemBlockedTags.AppendTags(ItemBlockedTags);
	SlotTemplate.ItemObject = nullptr;
	FixedSlots.Emplace(SlotTemplate);
	SlotCount++;
}

void UFixedSlotsInventoryComponent::RemoveSlot(FGameplayTagContainer ItemRequiredTags, FGameplayTagContainer ItemBlockedTags)
{
	int32 FoundIndex = FixedSlots.FindLastByPredicate([ItemRequiredTags, ItemBlockedTags](const FFixedInventorySlot& Slot)
	{
		return Slot.ItemBlockedTags == ItemRequiredTags && Slot.ItemBlockedTags == ItemBlockedTags && Slot.ItemObject == nullptr;
	});

	if (FixedSlots.IsValidIndex(FoundIndex))
	{
		FixedSlots.RemoveAt(FoundIndex);
	}
	SlotCount = FixedSlots.Num();
}