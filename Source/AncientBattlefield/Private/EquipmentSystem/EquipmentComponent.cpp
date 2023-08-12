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
	if (Item != nullptr && CanMoveTo(Item, SlotID))
	{
		Item->FindPropertyFragment<UPropertyFragment_Equipment>()->OnEquipmentPutOn(GetOwner(), (EEquipmentSlots)SlotID); // 实体显现-纯模型化-检测绑定
		Super::AddItem(Item, SlotID);
	}
}

void UEquipmentComponent::RemoveItem(UItemObject* Item)
{
	if (Item && Item->BelongingInventory == this && Item->BelongingSlotID < Size)
	{
		Item->FindPropertyFragment<UPropertyFragment_Equipment>()->OnEquipmentTakeOff(); // 检测解绑-实体销毁
		Super::RemoveItem(Item);
	}
}

bool UEquipmentComponent::CanMoveTo(UItemObject* Item, int32 SlotID) const
{
	if (auto Result = Super::CanMoveTo(Item, SlotID))
	{
		 if (UPropertyFragment_Equipment* EquipmentInfo = Item->FindPropertyFragment<UPropertyFragment_Equipment>())
		 {
			 return EquipmentInfo->PropertyFragment.EquipmentSlots.Contains((EEquipmentSlots)SlotID);
		 }
	}
	return false;
}

UItemObject* UEquipmentComponent::GetEquipment(EEquipmentSlots Slot)
{
	return Super::GetItem((int32)Slot);
}
