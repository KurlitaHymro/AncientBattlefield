// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSystem/PropertyFragment/PropertyFragment_MeleeWeapon.h"
#include "Anim/Components/HitTraceComponent.h"
#include "Item/ItemObject.h"
#include "PropertyFragment/PropertyFragment_EntityLink.h"
#include "EquipmentSystem/PropertyFragment/PropertyFragment_Equipment.h"

void UPropertyFragment_MeleeWeapon::Instantiate(UItemObject* Owner)
{
	Super::Instantiate(Owner);

	UPropertyFragment_Equipment* AsEquipment = Owner->FindPropertyFragment<UPropertyFragment_Equipment>();
	if (AsEquipment)
	{
		AsEquipment->ItemEquipmentPutOnDelegate.AddDynamic(this, &ThisClass::OnWeaponPutOn);
	}
}

void UPropertyFragment_MeleeWeapon::OnWeaponPutOn()
{
	auto Item = GetOwner();
	if (Item != nullptr)
	{
		UPropertyFragment_EntityLink* EntityLink = Item->FindPropertyFragment<UPropertyFragment_EntityLink>();
		if (EntityLink)
		{
			HitTraceComponent = EntityLink->GetEntity()->GetComponentByClass<UHitTraceComponent>();
		}
		UPropertyFragment_Equipment* AsEquipment = Item->FindPropertyFragment<UPropertyFragment_Equipment>();
		if (AsEquipment)
		{
			Mesh = AsEquipment->GetMesh();
		}

		if (HitTraceComponent && Mesh)
		{
			HitTraceComponent->Setup(Mesh);
		}
	}
}

void UPropertyFragment_MeleeWeapon::OnWeaponTakeOff()
{
	auto Item = GetOwner();
	if (Item != nullptr)
	{
		if (HitTraceComponent && Mesh)
		{
			HitTraceComponent->Teardown();
		}
	}
}

UHitTraceComponent* UPropertyFragment_MeleeWeapon::GetHitTraceComponent()
{
	return HitTraceComponent;
}
