// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSystem/PropertyFragment/PropertyFragment_Equipment.h"
#include "Item/ItemObject.h"
#include "PropertyFragment/PropertyFragment_EntityLink.h"
#include "PropertyFragment/PropertyFragment_PhysicsMesh.h"
#include "EquipmentSystem/PropertyFragment/PropertyFragment_MeleeWeapon.h"
#include "CombatCore/CombatCharacter.h"

void UPropertyFragment_Equipment::OnEquipmentPutOn()
{
	if (Owner != nullptr && Owner->BelongingInventory != nullptr && AttachSocket.IsValid())
	{
		auto BelongingCharacter = Cast<ACombatCharacter>(Owner->BelongingInventory->GetOwner());
		if (BelongingCharacter)
		{
			ParentMesh = BelongingCharacter->GetMesh();
		}
		if (ParentMesh)
		{
			// ʱ���Ͻ�ս��������ʵ���������ڣ��ʲ����ô���ȥ��Owner
			UPropertyFragment_EntityLink* EntityLink = Owner->FindPropertyFragment<UPropertyFragment_EntityLink>();
			if (EntityLink)
			{
				EntityLink->SpawnEntity();
				EquipmentEntity = EntityLink->GetEntity();
				if (EquipmentEntity)
				{
					FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
					EntityLink->GetEntity()->AttachToComponent(ParentMesh, Rules, AttachSocket);

					UPropertyFragment_PhysicsMesh* PhysicsMesh = Owner->FindPropertyFragment<UPropertyFragment_PhysicsMesh>();
					if (PhysicsMesh && PhysicsMesh->Mesh)
					{
						PhysicsMesh->SetEntityState(EEntityState::OnlyMesh);
					}

					UPropertyFragment_MeleeWeapon* MeleeWeapon = Owner->FindPropertyFragment<UPropertyFragment_MeleeWeapon>();
					if (MeleeWeapon)
					{
						MeleeWeapon->OnWeaponPutOn(this);
					}
				}
			}
		}
	}
}

void UPropertyFragment_Equipment::OnEquipmentTakeOff()
{
	if (EquipmentEntity != nullptr)
	{
		if (Owner != nullptr)
		{
			UPropertyFragment_MeleeWeapon* MeleeWeapon = Owner->FindPropertyFragment<UPropertyFragment_MeleeWeapon>();
			if (MeleeWeapon)
			{
				MeleeWeapon->OnWeaponTakeOff(this);
			}
			
			UPropertyFragment_EntityLink* EntityLink = Owner->FindPropertyFragment<UPropertyFragment_EntityLink>();
			if (EntityLink && EntityLink->GetEntity())
			{
				EntityLink->DestroyEntity();
				EquipmentEntity = nullptr;
			}
		}
	}
}

void UPropertyFragment_Equipment::PutOn()
{
	if (!Owner)
	{
		return;
	}
	UInventoryComponent* CharacterInventory = Owner->BelongingInventory;
	if (CharacterInventory)
	{
		// ��������Ʒ����������װ��������Ȼ���ƶ���Ʒ�������ȵĿ���λ�á�
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
				CharacterInventory->RemoveItem(Owner);
				CharacterEquipment->AddItem(Owner, (int32)EquipmentSlot);
			}
		}
	}
}

void UPropertyFragment_Equipment::TakeOff()
{
	if (!Owner)
	{
		return;
	}
	UEquipmentComponent* CharacterEquipment = Cast<UEquipmentComponent>(Owner->BelongingInventory);
	if (CharacterEquipment)
	{
		// ������װ��������������Ʒ������Ȼ���ƶ���Ʒ���ղۡ�
		UInventoryComponent* CharacterInventory = CharacterEquipment->GetOwner()->FindComponentByClass<UInventoryComponent>();
		if (CharacterInventory)
		{
			int32 SlotID = CharacterInventory->FindVacancy();
			if (SlotID < CharacterInventory->GetSize())
			{
				CharacterEquipment->RemoveItem(Owner);
				CharacterInventory->AddItem(Owner, SlotID);
			}
		}
	}
}