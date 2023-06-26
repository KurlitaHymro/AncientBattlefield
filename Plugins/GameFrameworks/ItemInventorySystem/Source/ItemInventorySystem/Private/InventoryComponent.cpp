// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Item/ItemObject.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

void UInventoryComponent::Setup(int32 SlotsNumber)
{
	Size = SlotsNumber;
	ItemObjectSlot.Init(nullptr, Size);
	OnSetup.Broadcast(Size);
}

int32 UInventoryComponent::FindVacancy() const
{
	int32 SlotID = 0;
	for (; SlotID < Size; SlotID++)
	{
		if (ItemObjectSlot[SlotID] == nullptr)
		{
			break;
		}
	}
	return SlotID;
}

void UInventoryComponent::AddItem(UItemObject* Item, int32 SlotID)
{
	if (Item && 0 <= SlotID && SlotID < Size)
	{
		ItemObjectSlot[SlotID] = Item;
		Item->BelongingInventory = this;
		Item->BelongingSlotID = SlotID;
		OnAddItem.Broadcast(Item, SlotID);
	}
}

void UInventoryComponent::RemoveItem(UItemObject* Item)
{
	if (Item && Item->BelongingInventory == this && Item->BelongingSlotID < Size)
	{
		int32 SlotID = Item->BelongingSlotID;
		Item->BelongingSlotID = Size;
		Item->BelongingInventory = nullptr;
		OnRemoveItem.Broadcast(Item, SlotID);
		ItemObjectSlot[SlotID] = nullptr;
	}
}

void UInventoryComponent::RemoveItemFromSlot(int32 SlotID)
{
	ItemObjectSlot.RangeCheck(SlotID);
	RemoveItem(ItemObjectSlot[SlotID]);
}

UItemObject* UInventoryComponent::GetItem(int32 SlotID)
{
	ItemObjectSlot.RangeCheck(SlotID);
	return ItemObjectSlot[SlotID];
}
