// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSystem/EquipmentComponent.h"
#include "Components/PawnComponent.h"
#include "Item/ItemObject.h"
#include "EquipmentSystem/PropertyFragment/PropertyFragment_Equipment.h"

void UEquipmentComponent::EquipmentSetup()
{
	Setup((int32)EEquipmentSlots::EquipmentSlotsNum);
}

void UEquipmentComponent::PutOnEquipment(UItemObject* Equipment, EEquipmentSlots Slot)
{
	if (Equipment != nullptr && Equipment->FindPropertyFragment<UPropertyFragment_Equipment>() != nullptr)
	{
		AddItem(Equipment, (int32)Slot);
		OnPutOnEquipment.Broadcast(Equipment, Slot);
	}
}

void UEquipmentComponent::TakeOffEquipment(UItemObject* Equipment)
{
	if (Equipment && Equipment->BelongingInventory == this && Equipment->BelongingSlotID < Size)
	{
		OnTakeOffEquipment.Broadcast(Equipment, (EEquipmentSlots)Equipment->BelongingSlotID);
		RemoveItem(Equipment);
	}
}

void UEquipmentComponent::TakeOffEquipmentFromSlot(EEquipmentSlots Slot)
{
	ItemObjectSlot.RangeCheck((int32)Slot);
	if (ItemObjectSlot[(int32)Slot] != nullptr)
	{
		OnTakeOffEquipment.Broadcast(ItemObjectSlot[(int32)Slot], Slot);
		RemoveItem(ItemObjectSlot[(int32)Slot]);
	}
}

UItemObject* UEquipmentComponent::GetEquipment(EEquipmentSlots Slot)
{
	return ItemObjectSlot[(int32)Slot];
}