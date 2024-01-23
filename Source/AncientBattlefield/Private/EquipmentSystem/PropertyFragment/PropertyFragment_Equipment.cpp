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
	auto EntityLink = Owner->FindPropertyFragment<UPropertyFragment_EntityLink>();
	EntityLink->ItemSpawnEntityDelegate.AddDynamic(this, &ThisClass::OnSpawnEntity);

	if (EquipMode)
	{
		Owner->ItemTagContainer.AddTag(EquipMode->ModeTag);
	}
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

FString UPropertyFragment_Equipment::GetPropertyDescription()
{
	return FString(TEXT("装备"));
}

FGameplayTagContainer UPropertyFragment_Equipment::GetRequiredTags()
{
	auto Tags = Super::GetRequiredTags();
	Tags.AddTag(UPropertyFragment_PhysicsMesh::PropertyTag);
	return Tags;
}

void UPropertyFragment_Equipment::PutOn()
{
	if (UInventoryComponent* Inventory = Owner->BelongingInventory)
	{
		for (int32 SlotID = 0; SlotID < Inventory->Slots.Num(); SlotID++)
		{
			if (Inventory->Slots[SlotID].Item == nullptr && Inventory->CanHold(Owner, SlotID) &&
				Inventory->Slots[SlotID].ItemRequiredTags.HasTag(PropertyTag))
			{
				Inventory->RemoveItem(Owner);
				Inventory->AddItemToSlot(Owner, SlotID);
			}
		}
	}
}

void UPropertyFragment_Equipment::TakeOff()
{
	if (UInventoryComponent* Inventory = Owner->BelongingInventory)
	{
		for (int32 SlotID = 0; SlotID < Inventory->Slots.Num(); SlotID++)
		{
			if (Inventory->Slots[SlotID].Item == nullptr && Inventory->CanHold(Owner, SlotID) &&
				!Inventory->Slots[SlotID].ItemRequiredTags.HasTag(PropertyTag))
			{
				Inventory->RemoveItem(Owner);
				Inventory->AddItemToSlot(Owner, SlotID);
			}
		}
	}
}

void UPropertyFragment_Equipment::SwitchMode()
{
	if (PropertyFragment.EquipModes.Num() <= 1)
	{
		return;
	}

	TakeOff();
	Owner->ItemTagContainer.RemoveTag(EquipMode->ModeTag);
	if (!++EquipMode)
	{
		EquipMode.Reset();
	}
	Owner->ItemTagContainer.AddTag(EquipMode->ModeTag);
	PutOn();
}

void UPropertyFragment_Equipment::OnInventoryUpdate(UInventoryComponent* Inventory, int32 LocalID)
{
	UPropertyFragment_EntityLink* EntityLink = Owner->FindPropertyFragment<UPropertyFragment_EntityLink>();
	if (Inventory && Inventory->Slots[LocalID].ItemRequiredTags.HasTag(PropertyTag))
	{
		EntityLink->SpawnEntity();
	}
	else if (ParentMesh)
	{
		EntityLink->DestroyEntity();
		ParentMesh = nullptr;
	}
}

void UPropertyFragment_Equipment::OnSpawnEntity(AActor* Entity)
{
	ensure(Entity && Owner && Owner->BelongingInventory && EquipMode);
	ParentMesh = Cast<ACharacter>(Owner->BelongingInventory->GetOwner())->GetMesh();

	FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	Entity->AttachToComponent(ParentMesh, Rules, EquipMode->AttachSocketName);

	UPropertyFragment_PhysicsMesh* PhysicsMesh = Owner->FindPropertyFragment<UPropertyFragment_PhysicsMesh>();
	if (PhysicsMesh)
	{
		UMeshComponent* Mesh = Entity->FindComponentByClass<UMeshComponent>();
		PhysicsMesh->SetEntityState(Mesh, EEntityState::OnlyMesh);
	}
}