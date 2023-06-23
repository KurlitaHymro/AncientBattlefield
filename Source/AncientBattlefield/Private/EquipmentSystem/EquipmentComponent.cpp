// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSystem/EquipmentComponent.h"
#include "Components/PawnComponent.h"
#include "Item/ItemObject.h"
#include "EquipmentSystem/PropertyFragment/PropertyFragment_Equipment.h"

UEquipmentComponent::UEquipmentComponent()
{
	
}

void UEquipmentComponent::InitEquipmentSlots()
{
	InitSlots((int32)EEquipmentSlots::EquipmentSlotsNum);
}

bool UEquipmentComponent::PutOnEquipment(UItemObject* Equipment, EEquipmentSlots Slot)
{
	if (Equipment != nullptr && Equipment->FindPropertyFragment<UPropertyFragment_Equipment>() != nullptr)
	{
		auto Handle = ConstructHandle((int)Slot);
		if (Handle.IsValid())
		{
			AddItem(Equipment, Handle);
			OnPutOnEquipment.Broadcast(Slot, Equipment);
			return true;
		}
	}
	return false;
}

bool UEquipmentComponent::TakeOffEquipment(EEquipmentSlots Slot)
{
	FItemSlotHandle Handle = ConstructHandle((int)Slot);
	if (Handle.IsValid())
	{
		OnTakeOffEquipment.Broadcast(Slot, GetItem(Handle));
		RemoveItem(Handle);
		
		return true;
	}
	return false;
}

UItemObject* UEquipmentComponent::GetEquipment(EEquipmentSlots Slot)
{
	return GetItem(ConstructHandle((int)Slot));
}