// Fill out your copyright notice in the Description page of Project Settings.


#include "Slot/ItemSlot.h"

namespace ItemSlotSet_Impl
{
	constexpr int32 InvalidID = -1;
	constexpr int32 StartID = 0;
}

FItemSlot::FItemSlot(int32 _SlotID) : SlotID(_SlotID)
{

}

FItemSlotSet::FItemSlotSet(UInventoryComponent* _Owner, int32 _SlotsNumber) : Owner(_Owner)
{
	for (int32 SlotID = ItemSlotSet_Impl::StartID; SlotID < ItemSlotSet_Impl::StartID + _SlotsNumber; SlotID++)
	{
		FItemSlot Slot(SlotID);
		ItemSlots.Emplace(Slot);
	}
}

FItemSlotHandle FItemSlotSet::AddItem(class UItemObject* Item)
{
	FItemSlotHandle Handle;
	Handle.Owner = Owner;
	Handle.SlotID = ItemSlotSet_Impl::InvalidID;
	for (auto Slot : ItemSlots)
	{
		if (Slot.Item == nullptr)
		{
			Slot.Item = Item;
			Handle.SlotID = Slot.SlotID;
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
