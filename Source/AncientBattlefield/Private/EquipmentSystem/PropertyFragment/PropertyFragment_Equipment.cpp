// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSystem/PropertyFragment/PropertyFragment_Equipment.h"
#include "DataRegistrySubsystem.h"
#include "InventoryComponent.h"
#include "Item/ItemObject.h"
#include "CombatCore/CombatCharacter.h"
#include "PropertyFragment/PropertyFragment_EntityLink.h"
#include "PropertyFragment/PropertyFragment_PhysicsMesh.h"
#include "EquipmentSystem/PropertyFragment/PropertyFragment_MeleeWeapon.h"

FGameplayTag UPropertyFragment_Equipment::PropertyTag(FGameplayTag::RequestGameplayTag(TEXT("InventorySystem.Property.Equipment")));
FName UPropertyFragment_Equipment::RegistryType(TEXT("EquipmentRegistry"));

void UPropertyFragment_Equipment::Init(const FName Template)
{
	Super::Init(Template);

	Owner->InventoryUpdateDelegate.AddDynamic(this, &ThisClass::OnInventoryUpdate);
}

void UPropertyFragment_Equipment::InitFromRegistry(FName Template)
{
	auto Registry = UDataRegistrySubsystem::Get()->GetRegistryForType(GetRegistryTypeName());
	if (Registry)
	{
		auto Prefab = Registry->GetCachedItem<FPropertyFragmentEquipment>(FDataRegistryId(GetRegistryTypeName(), Template));
		PropertyFragment = *Prefab;
	}
}

FGameplayTag UPropertyFragment_Equipment::GetPropertyTag()
{
	return PropertyTag;
}

FName UPropertyFragment_Equipment::GetRegistryTypeName()
{
	return RegistryType;
}

FGameplayTagContainer UPropertyFragment_Equipment::GetRequiredTags()
{
	auto Tags = Super::GetRequiredTags();
	Tags.AddTag(UPropertyFragment_PhysicsMesh::PropertyTag);
	return Tags;
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

void UPropertyFragment_Equipment::OnInventoryUpdate(UInventoryComponent* Inventory, int32 LocalID)
{
	UPropertyFragment_EntityLink* EntityLink = Owner->FindPropertyFragment<UPropertyFragment_EntityLink>();
	if (Inventory && Inventory->Slots[LocalID].ItemRequiredTags.HasTag(GetPropertyTag()))
	{
		if (EntityLink)
		{
			EntityLink->ItemSpawnEntityDelegate.AddDynamic(this, &ThisClass::OnSpawnEntity);
			EntityLink->SpawnEntity();
		}
	}
	else
	{
		if (EntityLink)
		{
			EntityLink->DestroyEntity();
			EntityLink->ItemSpawnEntityDelegate.RemoveAll(this);
			ParentMesh = nullptr;
		}
	}
}

void UPropertyFragment_Equipment::OnSpawnEntity(AActor* Entity)
{
	ensure(Entity && Owner && Owner->BelongingInventory);
	ACharacter* Char = Cast<ACharacter>(Owner->BelongingInventory->GetOwner());
	ParentMesh = Char->GetMesh();
	auto InitialMode = *PropertyFragment.EquipModes.begin();
	FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	Entity->AttachToComponent(ParentMesh, Rules, InitialMode.AttachSocketName);
	UPropertyFragment_PhysicsMesh* PhysicsMesh = Owner->FindPropertyFragment<UPropertyFragment_PhysicsMesh>();
	if (PhysicsMesh)
	{
		UMeshComponent* Mesh = Entity->FindComponentByClass<UMeshComponent>();
		PhysicsMesh->SetEntityState(Mesh, EEntityState::OnlyMesh);
	}
}