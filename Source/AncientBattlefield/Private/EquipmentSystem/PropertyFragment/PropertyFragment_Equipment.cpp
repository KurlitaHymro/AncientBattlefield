// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSystem/PropertyFragment/PropertyFragment_Equipment.h"
#include "DataRegistrySubsystem.h"
#include "Item/ItemObject.h"
#include "CombatCore/CombatCharacter.h"
#include "PropertyFragment/PropertyFragment_EntityLink.h"
#include "PropertyFragment/PropertyFragment_PhysicsMesh.h"
#include "EquipmentSystem/PropertyFragment/PropertyFragment_MeleeWeapon.h"

void UPropertyFragment_Equipment::InitFromDataTable(const UDataTable* DataTable, FName PrefabName)
{
	FPropertyFragmentEquipment* Prefab = DataTable->FindRow<FPropertyFragmentEquipment>(PrefabName, DataTable->GetName(), true);
	if (Prefab)
	{
		PropertyFragment = *Prefab;
	}
}

void UPropertyFragment_Equipment::InitFromRegistry(const FName RegistryType, FName PrefabName)
{
	auto Registry = UDataRegistrySubsystem::Get()->GetRegistryForType(RegistryType);
	if (Registry)
	{
		auto Prefab = Registry->GetCachedItem<FPropertyFragmentEquipment>(FDataRegistryId(RegistryType, PrefabName));
		PropertyFragment = *Prefab;
	}
}

FName UPropertyFragment_Equipment::GetPropertyTagName()
{
	return FName("InventorySystem.Property.Equipment");
}

FName UPropertyFragment_Equipment::GetRegistryTypeName()
{
	return FName("EquipmentRegistry");
}

void UPropertyFragment_Equipment::PutOn()
{
	UInventoryComponent* CharacterInventory = Owner->BelongingInventory;
	if (CharacterInventory)
	{
		// 查找与物品容器共属的装备容器，然后移动物品到最优先的可用位置。
		if (0)
		{
			EEquipmentSlots EquipmentSlot = EEquipmentSlots::EquipmentSlotsNum;
			//for (auto IdleSlot : PropertyFragment.EquipmentSlots)
			//{
			//	if (CharacterEquipment->GetItem((int32)IdleSlot.Key) == nullptr)
			//	{
			//		EquipmentSlot = IdleSlot.Key;
			//	}
			//}
			//if (EquipmentSlot < EEquipmentSlots::EquipmentSlotsNum)
			//{
			//	CharacterInventory->RemoveItem(Owner);
			//	CharacterEquipment->AddItem(Owner, (int32)EquipmentSlot);
			//}
		}
	}
}

void UPropertyFragment_Equipment::TakeOff()
{
	if (!Owner)
	{
		return;
	}
	//UEquipmentComponent* CharacterEquipment = Cast<UEquipmentComponent>(Owner->BelongingInventory);
	//if (CharacterEquipment)
	//{
	//	// 查找与装备容器共属的物品容器，然后移动物品到空槽。
	//	UInventoryComponent* CharacterInventory = CharacterEquipment->GetOwner()->FindComponentByClass<UInventoryComponent>();
	//	if (CharacterInventory)
	//	{
	//		int32 SlotID = CharacterInventory->FindVacancy();
	//		if (SlotID < CharacterInventory->GetSize())
	//		{
	//			CharacterEquipment->RemoveItem(Owner);
	//			CharacterInventory->AddItem(Owner, SlotID);
	//		}
	//	}
	//}
}

void UPropertyFragment_Equipment::OnEquipmentPutOn(AActor* Target, EEquipmentSlots TargetSlot)
{
	auto Character = Cast<ACombatCharacter>(Target);
	if (Character && Character->GetMesh() && !ParentMesh)
	{
		ParentMesh = Character->GetMesh();
		// 时序上近战武器依赖实体生命周期，故不能用代理去做Owner
		UPropertyFragment_EntityLink* EntityLink = Owner->FindPropertyFragment<UPropertyFragment_EntityLink>();
		if (EntityLink)
		{
			EntityLink->SpawnEntity();
			AActor* EquipmentEntity = EntityLink->GetEntity();
			auto Slot = PropertyFragment.EquipmentSlots.Find(TargetSlot);
			if (EquipmentEntity && Slot)
			{
				FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
				EquipmentEntity->AttachToComponent(ParentMesh, Rules, *Slot);

				UPropertyFragment_PhysicsMesh* PhysicsMesh = Owner->FindPropertyFragment<UPropertyFragment_PhysicsMesh>();
				if (PhysicsMesh)
				{
					UMeshComponent* Mesh = EquipmentEntity->FindComponentByClass<UMeshComponent>();
					PhysicsMesh->SetEntityState(Mesh, EEntityState::OnlyMesh);
				}

				UPropertyFragment_MeleeWeapon* MeleeWeapon = Owner->FindPropertyFragment<UPropertyFragment_MeleeWeapon>();
				if (MeleeWeapon)
				{
					MeleeWeapon->OnWeaponPutOn(Character->GetAbilitySystemComponent());
				}
			}
		}
	}
}

void UPropertyFragment_Equipment::OnEquipmentTakeOff()
{
	ParentMesh = nullptr;

	UPropertyFragment_MeleeWeapon* MeleeWeapon = Owner->FindPropertyFragment<UPropertyFragment_MeleeWeapon>();
	if (MeleeWeapon)
	{
		MeleeWeapon->OnWeaponTakeOff();
	}

	UPropertyFragment_EntityLink* EntityLink = Owner->FindPropertyFragment<UPropertyFragment_EntityLink>();
	if (EntityLink && EntityLink->GetEntity())
	{
		EntityLink->DestroyEntity();
	}
}