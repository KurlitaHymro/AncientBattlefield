// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSystem/PropertyFragment/PropertyFragment_MeleeWeapon.h"
#include "Anim/Components/HitTraceComponent.h"
#include "Item/ItemObject.h"
#include "PropertyFragment/PropertyFragment_EntityLink.h"
#include "PropertyFragment/PropertyFragment_PhysicsMesh.h"
#include "EquipmentSystem/PropertyFragment/PropertyFragment_Equipment.h"
#include "CombatCore/CombatCharacter.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"

void UPropertyFragment_MeleeWeapon::OnWeaponPutOn(UPropertyFragment_Equipment* Equipment)
{
	if (Owner != nullptr && Owner->BelongingInventory != nullptr)
	{
		auto BelongingCharacter = Cast<ACombatCharacter>(Owner->BelongingInventory->GetOwner());
		if (BelongingCharacter)
		{
			AbilitySystemComponent = Cast<UCombatAbilitySystemComponent>(BelongingCharacter->GetAbilitySystemComponent());
		}
		UPropertyFragment_EntityLink* EntityLink = Owner->FindPropertyFragment<UPropertyFragment_EntityLink>();
		if (EntityLink && EntityLink->GetEntity())
		{
			HitTraceComponent = EntityLink->GetEntity()->GetComponentByClass<UHitTraceComponent>();
			Mesh = EntityLink->GetEntity()->GetComponentByClass<UStaticMeshComponent>();
		}
		
		//UPropertyFragment_PhysicsMesh* PhysicsMesh = Item->FindPropertyFragment<UPropertyFragment_PhysicsMesh>();
		//if (PhysicsMesh && PhysicsMesh->Mesh)
		//{
		//	Mesh = PhysicsMesh->Mesh;
		//}
		if (AbilitySystemComponent != nullptr && HitTraceComponent != nullptr && Mesh != nullptr)
		{
			HitTraceComponent->Setup(Mesh);
			HitTraceComponent->UniqueHitDelegate.AddDynamic(this, &ThisClass::OnWeaponHit);
		}
	}
}

void UPropertyFragment_MeleeWeapon::OnWeaponTakeOff(UPropertyFragment_Equipment* Equipment)
{
	if (Owner != nullptr)
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