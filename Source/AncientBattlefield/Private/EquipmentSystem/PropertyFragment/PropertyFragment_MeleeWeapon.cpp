// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSystem/PropertyFragment/PropertyFragment_MeleeWeapon.h"
#include "Anim/Components/HitTraceComponent.h"
#include "Item/ItemObject.h"
#include "PropertyFragment/PropertyFragment_EntityLink.h"
#include "EquipmentSystem/PropertyFragment/PropertyFragment_Equipment.h"
#include "CombatCore/CombatCharacter.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"

void UPropertyFragment_MeleeWeapon::Instantiate(UItemObject* Owner)
{
	Super::Instantiate(Owner);

	UPropertyFragment_Equipment* EquipmentInfo = Owner->FindPropertyFragment<UPropertyFragment_Equipment>();
	if (EquipmentInfo)
	{
		EquipmentInfo->ItemEquipmentPutOnDelegate.AddDynamic(this, &ThisClass::OnWeaponPutOn);
		EquipmentInfo->ItemEquipmentTakeOffDelegate.AddDynamic(this, &ThisClass::OnWeaponTakeOff);
	}
}

void UPropertyFragment_MeleeWeapon::OnWeaponPutOn()
{
	auto Item = GetOwner();
	if (Item != nullptr && Item->BelongingInventory != nullptr)
	{
		auto OwnerCharacter = Cast<ACombatCharacter>(Item->BelongingInventory->GetOwner());
		if (OwnerCharacter)
		{
			AbilitySystemComponent = Cast<UCombatAbilitySystemComponent>(OwnerCharacter->GetAbilitySystemComponent());
		}
		UPropertyFragment_EntityLink* EntityLink = Item->FindPropertyFragment<UPropertyFragment_EntityLink>();
		if (EntityLink)
		{
			HitTraceComponent = EntityLink->GetEntity()->GetComponentByClass<UHitTraceComponent>();
		}
		UPropertyFragment_Equipment* EquipmentInfo = Item->FindPropertyFragment<UPropertyFragment_Equipment>();
		if (EquipmentInfo)
		{
			Mesh = EquipmentInfo->GetMesh();
		}

		if (AbilitySystemComponent && HitTraceComponent && Mesh)
		{
			HitTraceComponent->Setup(Mesh);
			HitTraceComponent->UniqueHitDelegate.AddDynamic(this, &ThisClass::OnWeaponHit);
		}
	}
}

void UPropertyFragment_MeleeWeapon::OnWeaponTakeOff()
{
	auto Item = GetOwner();
	if (Item != nullptr)
	{
		if (AbilitySystemComponent && HitTraceComponent && Mesh)
		{
			HitTraceComponent->Teardown();
			HitTraceComponent->UniqueHitDelegate.RemoveAll(this);
			AbilitySystemComponent = nullptr;
			HitTraceComponent = nullptr;
			Mesh = nullptr;
		}
	}
}

UHitTraceComponent* UPropertyFragment_MeleeWeapon::GetHitTraceComponent()
{
	return HitTraceComponent;
}

void UPropertyFragment_MeleeWeapon::OnWeaponHit(FHitResult HitResult)
{
	AbilitySystemComponent->HandleHitEvent(HitResult.GetActor());
}