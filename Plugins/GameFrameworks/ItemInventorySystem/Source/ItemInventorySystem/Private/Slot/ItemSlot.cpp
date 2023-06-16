// Fill out your copyright notice in the Description page of Project Settings.


#include "Slot/ItemSlot.h"

namespace ItemSlotSet_Impl
{
	constexpr int32 InvalidID = -1;
	constexpr int32 StartID = 0;
}

void FItemSlotSet::Init(UInventoryComponent* OwnerInventoryComponent, int32 SlotsNumber)
{
	Owner = OwnerInventoryComponent;
	Size = SlotsNumber;
	ItemSlots.Init(FItemSlot(), Size);
}

FItemSlotHandle FItemSlotSet::AddItem(class UItemObject* Item)
{
	FItemSlotHandle Handle;
	Handle.Owner = Owner;
	Handle.SlotID = ItemSlotSet_Impl::InvalidID;
	for (int32 SlotID = ItemSlotSet_Impl::StartID; SlotID < Size; SlotID++)
	{
		if (ItemSlots[SlotID].Item == nullptr)
		{
			ItemSlots[SlotID].Item = Item;
			Handle.SlotID = SlotID;
			break;
		}
	}
	return Handle;
}

bool FItemSlotSet::RemoveItem(FItemSlotHandle Handle)
{
	if (Handle.IsValid() && Handle.Owner == Owner)
	{
		ItemSlots[Handle.SlotID].Item = nullptr;
		return true;
	}
	return false;
}

void FItemSlotSet::SetItem(FItemSlotHandle Handle, UItemObject* Item)
{
	if (Handle.IsValid() && Handle.Owner == Owner)
	{
		ItemSlots[Handle.SlotID].Item = Item;
	}
}

UItemObject* FItemSlotSet::FindItem(FItemSlotHandle Handle)
{
	if (Handle.IsValid() && Handle.Owner == Owner)
	{
		return ItemSlots[Handle.SlotID].Item;
	}
	return nullptr;
}

bool FItemSlotHandle::IsValid()
{
	return Owner != nullptr && SlotID != ItemSlotSet_Impl::InvalidID;
}
