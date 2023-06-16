// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSystem/EquipmentComponent.h"
#include "Components/PawnComponent.h"

void UEquipmentComponent::InitEquipmentSlots()
{
	InitSlots((int32)EEquipmentSlots::EquipmentSlotsNum);
	for (int32 Index = (int32)EEquipmentSlots::Helmet; Index < (int32)EEquipmentSlots::EquipmentSlotsNum; Index++)
	{
		SlotHandles.Add(AddItem());
	}
}

bool UEquipmentComponent::PutOnEquipment(UItemObject* Equipment, EEquipmentSlots Slot)
{
	if (Equipment != nullptr)
	{
		auto Handle = SlotHandles[(int32)Slot];
		if (Handle.IsValid())
		{
			AddItem(Equipment, Handle);
		}
		return true;
	}
	return false;
}

bool UEquipmentComponent::TakeOffEquipment(EEquipmentSlots Slot)
{
	FItemSlotHandle Handle = SlotHandles[(int32)Slot];
	if (Handle.IsValid())
	{
		RemoveItem(Handle);
		return true;
	}
	return false;
}

UItemObject* UEquipmentComponent::GetEquipment(EEquipmentSlots Slot)
{
	return GetItem(SlotHandles[(int32)Slot]);
}
