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
			// �����������Ʒ����������װ��������
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
	auto Item = GetOwner();
	if (Item != nullptr)
	{
		UPropertyFragment_EntityLink* EntityLink = Item->FindPropertyFragment<UPropertyFragment_EntityLink>();
		if (EntityLink)
		{
			EntityLink->DestroyEntity();
		}
	}

	OnEquipmentTakeOff.Broadcast(); // ������ƽ��֪ͨ��������Ƭ��
	UEquipmentComponent* CharacterEquipment = Cast<UEquipmentComponent>(Item->BelongingInventory);
	if (CharacterEquipment)
	{
		// ���������װ��������������Ʒ������
		UInventoryComponent* CharacterInventory = CharacterEquipment->GetOwner()->FindComponentByClass<UInventoryComponent>();
		if (CharacterInventory)
		{
			int32 SlotID = CharacterInventory->FindVacancy();
			if (SlotID < CharacterInventory->GetSize())
			{
				CharacterEquipment->TakeOffEquipment(Item);
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
