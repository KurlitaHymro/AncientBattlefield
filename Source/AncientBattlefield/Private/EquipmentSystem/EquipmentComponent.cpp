// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSystem/EquipmentComponent.h"
#include "Components/PawnComponent.h"
#include "Item/ItemObject.h"
#include "EquipmentSystem/PropertyFragment/PropertyFragment_Equipment.h"

void UEquipmentComponent::Setup(int32 SlotsNumber)
{
	Super::Setup((int32)EEquipmentSlots::EquipmentSlotsNum);
}

void UEquipmentComponent::AddItem(UItemObject* Item, int32 SlotID)
{
	if (Item != nullptr && Item->FindPropertyFragment<UPropertyFragment_Equipment>() != nullptr)
	{
		Item->FindPropertyFragment<UPropertyFragment_Equipment>()->ItemEquipmentPutOnDelegate.Broadcast();
		Super::AddItem(Item, SlotID);
	}
}

void UEquipmentComponent::RemoveItem(UItemObject* Item)
{
	if (Item && Item->BelongingInventory == this && Item->BelongingSlotID < Size)
	{
		Item->FindPropertyFragment<UPropertyFragment_Equipment>()->ItemEquipmentTakeOffDelegate.Broadcast();
		Super::RemoveItem(Item);
	}
}

UItemObject* UEquipmentComponent::GetEquipment(EEquipmentSlots Slot)
{
	return Super::GetItem((int32)Slot);
}
