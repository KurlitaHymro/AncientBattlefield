// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Item/ItemObject.h"

const int32 UInventoryComponent::InvalidIndex(-1);

bool FInventorySlot::CanHold(UItemObject* ExternItem) const
{
	FGameplayTagContainer ItemTags;
	ExternItem->GetOwnedGameplayTags(ItemTags);
	return ItemTags.HasAll(ItemRequiredTags) && !ItemTags.HasAny(ItemBlockedTags);
}

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Slots.Empty();
}

UItemObject* UInventoryComponent::GetItem(int32 SlotID)
{
	Slots.RangeCheck(SlotID);
	return Slots[SlotID].Item;
}

bool UInventoryComponent::CanHold(UItemObject* Item, int32 SlotID) const
{
	if (Slots.IsValidIndex(SlotID))
	{
		return Slots[SlotID].CanHold(Item);
	}
	return false;
}

bool UInventoryComponent::AddItem(UItemObject* Item)
{
	for (int32 SlotID = 0; SlotID < Slots.Num(); SlotID++)
	{
		if (Slots[SlotID].Item == nullptr && CanHold(Item, SlotID))
		{
			return AddItemToSlot(Item, SlotID);
		}
	}
	return false;
}

bool UInventoryComponent::AddItemToSlot(UItemObject* Item, int32 SlotID)
{
	if (Slots.IsValidIndex(SlotID) && Slots[SlotID].Item == nullptr && CanHold(Item, SlotID))
	{
		Slots[SlotID].Item = Item;
		InventoryAddItemDelegate.Broadcast(Item, SlotID);
		Item->InventoryUpdateDelegate.Broadcast(this, SlotID);
		return true;
	}
	return false;
}

bool UInventoryComponent::RemoveItem(UItemObject* Item)
{
	if (Item && Item->BelongingInventory == this && Slots.IsValidIndex(Item->BelongingSlotID) && Slots[Item->BelongingSlotID].Item == Item)
	{
		Slots[Item->BelongingSlotID].Item = nullptr;
		InventoryRemoveItemDelegate.Broadcast(Item, Item->BelongingSlotID);
		Item->InventoryUpdateDelegate.Broadcast(nullptr, InvalidIndex);
		return true;
	}
	return false;
}

bool UInventoryComponent::RemoveItemFromSlot(int32 SlotID)
{
	return Slots.IsValidIndex(SlotID) ? RemoveItem(Slots[SlotID].Item) : false;
}

// 交换
bool UInventoryComponent::SwapItems(UItemObject* Item1, UItemObject* Item2)
{
	if (Item1 && Item1->BelongingInventory && Item1->BelongingInventory->Slots.IsValidIndex(Item1->BelongingSlotID) &&
		Item2 && Item2->BelongingInventory && Item2->BelongingInventory->Slots.IsValidIndex(Item2->BelongingSlotID))
	{
		UInventoryComponent* Inventory1 = Item1->BelongingInventory;
		UInventoryComponent* Inventory2 = Item2->BelongingInventory;
		int32 SlotID1 = Item1->BelongingSlotID;
		int32 SlotID2 = Item2->BelongingSlotID;
		if (Inventory1->CanHold(Item2, SlotID1) && Inventory2->CanHold(Item1, SlotID2))
		{
			Inventory1->Slots[SlotID1].Item = Item2;
			Inventory2->Slots[SlotID2].Item = Item1;
			Inventory1->InventoryRemoveItemDelegate.Broadcast(Item1, SlotID1);
			Inventory2->InventoryRemoveItemDelegate.Broadcast(Item2, SlotID2);
			Inventory1->InventoryAddItemDelegate.Broadcast(Item2, SlotID1);
			Inventory2->InventoryAddItemDelegate.Broadcast(Item1, SlotID2);
			Item1->InventoryUpdateDelegate.Broadcast(Inventory2, SlotID2);
			Item2->InventoryUpdateDelegate.Broadcast(Inventory1, SlotID1);
			return true;
		}
	}
	return false;
}

// 简单整理 O(n^2)
bool UInventoryComponent::CollectItems()
{
	TArray<TObjectPtr<UItemObject>> Items;
	for (auto Slot : Slots)
	{
		if (Slot.Item)
		{
			Items.Add(Slot.Item);
			RemoveItem(Slot.Item);
		}
	}
	auto Iterator = Items.CreateIterator();
	while (Iterator)
	{
		if (AddItem(*Iterator))
		{
			Iterator++;
		}
		else
		{
			return false;
		}
	}
	return true;
}

FString UInventoryComponent::GetStaticDescription() const
{
	FString Description = FString::Printf(TEXT("%s::%s"), *GetOwner()->GetName(), *this->GetName());
	for (int32 i = 0; i < Slots.Num(); i++)
	{
		if (i % 10)
		{
			Description += FString::Printf(TEXT("\n"));
		}
		if (Slots[i].Item)
		{
			Description += FString::Printf(TEXT("[%02d]{% 10s} "), i, *Slots[i].Item->GetFName().ToString());
		}
		else
		{
			Description += FString::Printf(TEXT("[%02d]{      NULL} "), i);
		}
	}
	return Description;
} 