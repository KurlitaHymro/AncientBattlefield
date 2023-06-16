// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

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
	}
	return Handle;
}

bool UInventoryComponent::RemoveItem(FItemSlotHandle SlotHandle)
{
	UItemObject* Item = SlotSet.FindItem(SlotHandle);

	if (Item != nullptr)
	{
		OnRemoveItem.Broadcast(Item, SlotHandle.SlotID);
	}

	return SlotSet.RemoveItem(SlotHandle);
}

UItemObject* UInventoryComponent::GetItem(FItemSlotHandle SlotHandle)
{
	return SlotSet.FindItem(SlotHandle);
}
