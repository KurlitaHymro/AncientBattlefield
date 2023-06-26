// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSystem/PropertyFragment/PropertyFragment_Equipment.h"
#include "Item/ItemObject.h"
#include "PropertyFragment/PropertyFragment_EntityLink.h"

void UPropertyFragment_Equipment::Instantiate(UItemObject* Owner)
{
	Super::Instantiate(Owner);

}

void UPropertyFragment_Equipment::PutOn()
{
	auto Item = GetOwner();
	if (Item != nullptr)
	{
		UPropertyFragment_EntityLink* EntityLink = Item->FindPropertyFragment<UPropertyFragment_EntityLink>();
		if (EntityLink)
		{
			EquipmentEntity = EntityLink->GetEntity();
		}
	}
	
	if (ParentMesh != nullptr && EquipmentEntity != nullptr && AttachSocket.IsValid())
	{
		FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
		EquipmentEntity->AttachToComponent(ParentMesh, Rules, AttachSocket);
		OnEquipmentPutOn.Broadcast(); // ������ƽ��֪ͨ��������Ƭ��

		UInventoryComponent* CharacterInventory = Item->BelongingInventory;
		if (CharacterInventory)
		{
			// �����Ǿ��й�������Ʒ����ͬʱ��������Ʒ����������װ��������
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
					CharacterEquipment->PutOnEquipment(Item, EquipmentSlot);
				}
			}
		}
	}
}

void UPropertyFragment_Equipment::TakeOff()
{
	
}

UMeshComponent* UPropertyFragment_Equipment::GetMesh()
{
	if (EquipmentEntity && EquipmentMesh == nullptr)
	{
		EquipmentMesh = EquipmentEntity->FindComponentByClass<UMeshComponent>();
	}
	return EquipmentMesh;
}
