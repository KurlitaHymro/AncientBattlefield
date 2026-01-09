// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSubsystem/PropertyFragment_Equipment.h"
#include "EntitiesSubsystem/PropertyFragment_EntityLink.h"
#include "InventorySubsystem/PropertyFragment_Inventory.h"
#include "InventorySubsystem/Components/InventoryComponent.h"
#include "InventorySubsystem/Components/FixedSlotsInventoryComponent.h"
#include "InventorySubsystem/InventorySystemInterface.h"
#include "DataRegistrySubsystem.h"
#include "Item/ItemObject.h"
#include "ItemSystem.h"

#define LOCTEXT_NAMESPACE "PropertyFragment_Equipment"

FName UPropertyFragment_Equipment::RegistryType(TEXT("EquipmentRegistry"));

void UPropertyFragment_Equipment::Setup(const FName Template)
{
	Super::Setup(Template);

	if (UPropertyFragment_Inventory* Inventory = OwnerItem->FindPropertyFragment<UPropertyFragment_Inventory>())
	{
		Inventory->InventoryUpdateDelegate.AddDynamic(this, &ThisClass::OnInventoryUpdate);
	}
}

void UPropertyFragment_Equipment::Teardown()
{
	if (UPropertyFragment_Inventory* Inventory = OwnerItem->FindPropertyFragment<UPropertyFragment_Inventory>())
	{
		Inventory->InventoryUpdateDelegate.RemoveDynamic(this, &ThisClass::OnInventoryUpdate);
	}

	Super::Teardown();
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

bool UPropertyFragment_Equipment::IsKeyMatch(FName Key)
{
	return Key == PropertyFragment.AttachSocketName;
}

FGameplayTag UPropertyFragment_Equipment::GetPropertyTag()
{
	return ItemSystemGameplayTags::ItemSystem_Property_Equipment;
}

FName UPropertyFragment_Equipment::GetRegistryTypeName()
{
	return RegistryType;
}

FGameplayTagContainer UPropertyFragment_Equipment::GetRequiredTags()
{
	FGameplayTagContainer Container = Super::GetRequiredTags();
	Container.AddTag(ItemSystemGameplayTags::ItemSystem_Property_EntityLink);
	return Container;
}

void UPropertyFragment_Equipment::Equip()
{
	UPropertyFragment_Inventory* Inventory = OwnerItem->FindPropertyFragment<UPropertyFragment_Inventory>();
	ensureAlways(Inventory && Inventory->Inventory);

	TArray<UFixedSlotsInventoryComponent*> FixedSlotInventoryComponents;
	Inventory->Inventory->GetOwner()->GetComponents(FixedSlotInventoryComponents);
	for (UFixedSlotsInventoryComponent* TargetComponent : FixedSlotInventoryComponents)
	{
		int32 SlotIndex = TargetComponent->FindAvailableSlot(OwnerItem);
		if (TargetComponent->FixedSlots.IsValidIndex(SlotIndex) && TargetComponent->FixedSlots[SlotIndex].ItemRequiredTags.HasTag(GetPropertyTag()))
		{
			Inventory->Inventory->Remove(OwnerItem);
			TargetComponent->AddTo(SlotIndex, OwnerItem);
			return;
		}
	}
	UE_LOG(LogItemSystem, Warning, TEXT("No available inventory or slot to Equip"))
}

void UPropertyFragment_Equipment::Unequip()
{
	UPropertyFragment_Inventory* Inventory = OwnerItem->FindPropertyFragment<UPropertyFragment_Inventory>();
	ensureAlways(Inventory && Inventory->Inventory);

	AActor* OwnerActor = Inventory->Inventory->GetOwner();
	IInventorySystemInterface* InventoryInterface = Cast<IInventorySystemInterface>(OwnerActor);
	if (InventoryInterface)
	{
		UInventoryComponent* TargetComponent = InventoryInterface->GetInventoryComponent();
		Inventory->Inventory->Remove(OwnerItem);
		TargetComponent->Add(OwnerItem);
		return;
	}
	else
	{
		TArray<UInventoryComponent*> InventoryComponents;
		OwnerActor->GetComponents(InventoryComponents);
		for (UInventoryComponent* Component : InventoryComponents)
		{
			if (UFixedSlotsInventoryComponent* TargetComponent = Cast<UFixedSlotsInventoryComponent>(Component))
			{
				int32 SlotIndex = TargetComponent->FindAvailableSlot(OwnerItem);
				if (TargetComponent->FixedSlots.IsValidIndex(SlotIndex) && !TargetComponent->FixedSlots[SlotIndex].ItemRequiredTags.HasTag(GetPropertyTag()))
				{
					Inventory->Inventory->Remove(OwnerItem);
					TargetComponent->AddTo(SlotIndex, OwnerItem);
					return;
				}
			}
			else
			{
				Inventory->Inventory->Remove(OwnerItem);
				Component->Add(OwnerItem);
				return;
			}
		}
	}
	UE_LOG(LogItemSystem, Warning, TEXT("No available inventory or slot to Detach"))
}

void UPropertyFragment_Equipment::OnInventoryUpdate(UInventoryComponent* InventoryComponent, int32 Special)
{
	UPropertyFragment_Inventory* Inventory = OwnerItem->FindPropertyFragment<UPropertyFragment_Inventory>();
	ensureAlways(Inventory);

	if (InventoryComponent)
	{
		if (UFixedSlotsInventoryComponent* FixedSlotInventoryComponent = Cast<UFixedSlotsInventoryComponent>(InventoryComponent))
		{
			if (FixedSlotInventoryComponent->FixedSlots.IsValidIndex(Special) && FixedSlotInventoryComponent->FixedSlots[Special].ItemRequiredTags.HasTag(GetPropertyTag()))
			{
				SpawnAttachEquipmentEntity();
				Inventory->RegisterInventoryOperation(0, this, FName("Unequip"), LOCTEXT("PropertyOperation_Unequip", "Unequip"));
				return;
			}
		}
		DestroyEquipmentEntity();
		Inventory->RegisterInventoryOperation(0, this, FName("Equip"), LOCTEXT("PropertyOperation_Equip", "Equip"));
		return;
	}
	else
	{
		DestroyEquipmentEntity();
		Inventory->UnregisterInventoryOperation(0);
		return;
	}
}

void UPropertyFragment_Equipment::SpawnAttachEquipmentEntity(UPrimitiveComponent* Parent)
{
	UPrimitiveComponent* AttachComponent = Parent;
	if (!AttachComponent)
	{
		/**
		 * 理论上，装备可以挂到任何地方，只要传入的组件是合理的。
		 * 否则自动查找组件，会从物品的库存子系统，查找库存组件持有者的所有基元组件。
		 */
		if (UPropertyFragment_Inventory* Inventory = Cast<UPropertyFragment_Inventory>(OwnerItem->FindPropertyFragment<UPropertyFragment_Inventory>()))
		{
			if (UInventoryComponent* InventoryComponent = Inventory->Inventory)
			{
				TArray<UPrimitiveComponent*> PrimitiveComponents;
				InventoryComponent->GetOwner()->GetComponents(PrimitiveComponents);
				for (UPrimitiveComponent* Component : PrimitiveComponents)
				{
					if (Component->DoesSocketExist(PropertyFragment.AttachSocketName))
					{
						AttachComponent = Component;
						break;
					}
				}
			}
		}
	}

	UPropertyFragment_EntityLink* EntityLink = Cast<UPropertyFragment_EntityLink>(OwnerItem->FindPropertyFragment<UPropertyFragment_EntityLink>());
	if (AttachComponent && EntityLink)
	{
		EntityLink->SpawnEntity();
		if (AActor* Entity = EntityLink->GetEntity())
		{
			FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
			Entity->AttachToComponent(AttachComponent, AttachmentRules, PropertyFragment.AttachSocketName);
		}
	}
}

void UPropertyFragment_Equipment::DestroyEquipmentEntity()
{
	UPropertyFragment_EntityLink* EntityLink = Cast<UPropertyFragment_EntityLink>(OwnerItem->FindPropertyFragment<UPropertyFragment_EntityLink>());
	if (EntityLink)
	{
		EntityLink->DestroyEntity();
	}
}

#undef LOCTEXT_NAMESPACE