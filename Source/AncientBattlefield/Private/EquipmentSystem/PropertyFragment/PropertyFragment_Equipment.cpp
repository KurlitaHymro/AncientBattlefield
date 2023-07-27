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
			// 时序上近战武器依赖实体生命周期，故不能用代理去做Owner
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
		// 查找与装备容器共属的物品容器，然后移动物品到空槽。
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