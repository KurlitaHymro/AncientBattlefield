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

void UInventoryComponent::InitSlots(int32 SlotsNumber)
{
	SlotSet.Init(this, SlotsNumber);

	if (SlotsNumber > 0)
	{
		OnInitSlot.Broadcast(SlotsNumber);
	}
}

FItemSlotHandle UInventoryComponent::AddItem(UItemObject* Item, FItemSlotHandle SlotHandle)
{
	FItemSlotHandle Handle;
	if (SlotHandle.IsValid())
	{
		Handle = SlotHandle;
		if (SlotSet.FindItem(Handle) != nullptr)
		{
			RemoveItem(Handle);
		}
		SlotSet.SetItem(Handle, Item);
	}
	else
	{
		Handle = SlotSet.AddItem(Item);
	}
	if (Handle.IsValid())
	{
		OnAddItem.Broadcast(Item, Handle.SlotID);
		Item->OnAddToInventory.Broadcast(this);
	}
	return Handle;
}

bool UInventoryComponent::RemoveItem(FItemSlotHandle SlotHandle)
{
	UItemObject* Item = SlotSet.FindItem(SlotHandle);

	if (Item != nullptr)
	{
		OnRemoveItem.Broadcast(Item, SlotHandle.SlotID);
		Item->OnRemoveFromInventory.Broadcast(this);
	}

	return SlotSet.RemoveItem(SlotHandle);
}

UItemObject* UInventoryComponent::GetItem(FItemSlotHandle SlotHandle)
{
	return SlotSet.FindItem(SlotHandle);
}

FItemSlotHandle UInventoryComponent::ConstructHandle(int32 SlotID)
{
	FItemSlotHandle Handle;
	Handle.Owner = this;
	Handle.SlotID = SlotID;
	return Handle;
}
