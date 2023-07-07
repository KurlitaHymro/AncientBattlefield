// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSystem/PropertyFragment/PropertyFragment_Equipment.h"
#include "Item/ItemObject.h"
#include "PropertyFragment/PropertyFragment_EntityLink.h"

void UPropertyFragment_Equipment::Instantiate(UItemObject* Owner)
{
	Super::Instantiate(Owner);

	ItemEquipmentPutOnDelegate.AddDynamic(this, &ThisClass::OnEquipmentPutOn);
	ItemEquipmentTakeOffDelegate.AddDynamic(this, &ThisClass::OnEquipmentTakeOff);
}

void UPropertyFragment_Equipment::OnEquipmentPutOn()
{
	if (ParentMesh != nullptr && AttachSocket.IsValid())
	{
		auto Item = GetOwner();
		if (Item != nullptr)
		{
			UPropertyFragment_EntityLink* EntityLink = Item->FindPropertyFragment<UPropertyFragment_EntityLink>();
			if (EntityLink)
			{
				EquipmentEntity = EntityLink->GetEntity();
				FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
				EquipmentEntity->AttachToComponent(ParentMesh, Rules, AttachSocket);
			}
		}
	}
}

void UPropertyFragment_Equipment::OnEquipmentTakeOff()
{
	if (EquipmentEntity != nullptr)
	{
		auto Item = GetOwner();
		if (Item != nullptr)
		{
			UPropertyFragment_EntityLink* EntityLink = Item->FindPropertyFragment<UPropertyFragment_EntityLink>();
			if (EntityLink)
			{
				EntityLink->DestroyEntity();
			}
		}
	}
}

void UPropertyFragment_Equipment::PutOn()
{
	auto Item = GetOwner();
	if (!Item)
	{
		return;
	}
	UInventoryComponent* CharacterInventory = Item->BelongingInventory;
	if (CharacterInventory)
	{
		// 查找与物品容器共属的装备容器，然后移动物品到最优先的可用位置。
		UEquipmentComponent* CharacterEquipment = CharacterInventory->GetOwner()->FindComponentByClass<UEquipmentComponent>();
		if (CharacterEquipment)
		{
			EEquipmentSlots EquipmentSlot = EEquipmentSlots::EquipmentSlotsNum;
			for (auto IdleSlot : RestrictSlot)
			{
				if (CharacterEquipment->GetItem((int32)IdleSlot) == nullptr)
				{
					EquipmentSlot = IdleSlot;
				}
			}
			if (EquipmentSlot < EEquipmentSlots::EquipmentSlotsNum)
			{
				CharacterInventory->RemoveItem(Item);
				CharacterEquipment->AddItem(Item, (int32)EquipmentSlot);
			}
		}
	}
}

void UPropertyFragment_Equipment::TakeOff()
{
	auto Item = GetOwner();
	if (!Item)
	{
		return;
	}
	UEquipmentComponent* CharacterEquipment = Cast<UEquipmentComponent>(Item->BelongingInventory);
	if (CharacterEquipment)
	{
		// 查找与装备容器共属的物品容器，然后移动物品到空槽。
		UInventoryComponent* CharacterInventory = CharacterEquipment->GetOwner()->FindComponentByClass<UInventoryComponent>();
		if (CharacterInventory)
		{
			int32 SlotID = CharacterInventory->FindVacancy();
			if (SlotID < CharacterInventory->GetSize())
			{
				CharacterEquipment->RemoveItem(Item);
				CharacterInventory->AddItem(Item, SlotID);
			}
		}
	}
}

UMeshComponent* UPropertyFragment_Equipment::GetMesh()
{
	if (EquipmentEntity && EquipmentMesh == nullptr)
	{
		EquipmentMesh = EquipmentEntity->FindComponentByClass<UMeshComponent>();
	}
	return EquipmentMesh;
}
