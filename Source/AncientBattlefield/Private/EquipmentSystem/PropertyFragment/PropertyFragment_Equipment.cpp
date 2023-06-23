// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSystem/PropertyFragment/PropertyFragment_Equipment.h"
#include "Item/ItemObject.h"
#include "PropertyFragment/PropertyFragment_EntityLink.h"

void UPropertyFragment_Equipment::Instantiate(UItemObject* Owner)
{
	Super::Instantiate(Owner);

	Owner->OnAddToInventory.AddDynamic(this, &ThisClass::OnAddToInventoryComponent);
}

void UPropertyFragment_Equipment::OnAddToInventoryComponent(UInventoryComponent* InventoryComponent)
{
	auto EquipmentComponent = Cast<UEquipmentComponent>(InventoryComponent);
	if (EquipmentComponent)
	{
		EquipmentComponent->OnPutOnEquipment.AddDynamic(this, &ThisClass::PutOn);
	}
}

void UPropertyFragment_Equipment::PutOn(EEquipmentSlots Slot, UItemObject* Item)
{
	if (Item != nullptr)
	{
		UPropertyFragment_EntityLink* EntityLink = Item->FindPropertyFragment<UPropertyFragment_EntityLink>();
		if (EntityLink)
		{
			if (EntityLink->GetEntity() == nullptr)
			{
				EntityLink->SpawnEntity();
			}
			EquipmentMesh = EntityLink->GetEntity()->GetComponentByClass<UMeshComponent>();
		}
	}

	if (ParentMesh != nullptr && EquipmentMesh != nullptr && AttachSocket.IsValid())
	{
		FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
		EquipmentMesh->AttachToComponent(ParentMesh, Rules, AttachSocket);
	}
}

void UPropertyFragment_Equipment::TakeOff(EEquipmentSlots Slot, UItemObject* Item)
{
}

UMeshComponent* UPropertyFragment_Equipment::GetMesh()
{
	return EquipmentMesh;
}
