// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSystem/PropertyFragment/PropertyFragment_MeleeWeapon.h"
#include "Anim/Components/HitTraceComponent.h"
#include "Item/ItemObject.h"
#include "PropertyFragment/PropertyFragment_EntityLink.h"

void UPropertyFragment_MeleeWeapon::Instantiate(UItemObject* Owner)
{
	Super::Instantiate(Owner);

	Owner->OnAddToInventory.AddDynamic(this, &ThisClass::OnAddToInventoryComponent);
}

void UPropertyFragment_MeleeWeapon::OnAddToInventoryComponent(UInventoryComponent* InventoryComponent)
{
	auto EquipmentComponent = Cast<UEquipmentComponent>(InventoryComponent);
	if (EquipmentComponent)
	{
		EquipmentComponent->OnPutOnEquipment.AddDynamic(this, &ThisClass::PutOn);
	}
}

void UPropertyFragment_MeleeWeapon::PutOn(EEquipmentSlots Slot, UItemObject* Item)
{
	if (Item != nullptr)
	{
		UPropertyFragment_EntityLink* EntityLink = Item->FindPropertyFragment<UPropertyFragment_EntityLink>();
		if (EntityLink)
		{
			HitTraceComponent = EntityLink->GetEntity()->GetComponentByClass<UHitTraceComponent>();
		}
	}
}

void UPropertyFragment_MeleeWeapon::TakeOff(EEquipmentSlots Slot, UItemObject* Item)
{

}

UHitTraceComponent* UPropertyFragment_MeleeWeapon::GetHitTraceComponent()
{
	return HitTraceComponent;
}
