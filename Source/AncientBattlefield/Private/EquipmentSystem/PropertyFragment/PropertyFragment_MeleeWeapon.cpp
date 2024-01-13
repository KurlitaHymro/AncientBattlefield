// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSystem/PropertyFragment/PropertyFragment_MeleeWeapon.h"
#include "DataRegistrySubsystem.h"
#include "Anim/Components/HitTraceComponent.h"
#include "Anim/Components/TrailingComponent.h"
#include "InventoryComponent.h"
#include "Item/ItemObject.h"
#include "PropertyFragment/PropertyFragment_EntityLink.h"
#include "PropertyFragment/PropertyFragment_PhysicsMesh.h"
#include "EquipmentSystem/PropertyFragment/PropertyFragment_Equipment.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"

void UPropertyFragment_MeleeWeapon::InitFromDataTable(const UDataTable* DataTable, FName PrefabName)
{
	FPropertyFragmentMeleeWeapon* Prefab = DataTable->FindRow<FPropertyFragmentMeleeWeapon>(PrefabName, DataTable->GetName(), true);
	if (Prefab)
	{
		PropertyFragment = *Prefab;
	}
}

void UPropertyFragment_MeleeWeapon::InitFromRegistry(const FName RegistryType, FName PrefabName)
{
	auto Registry = UDataRegistrySubsystem::Get()->GetRegistryForType(RegistryType);
	if (Registry)
	{
		auto Prefab = Registry->GetCachedItem<FPropertyFragmentMeleeWeapon>(FDataRegistryId(RegistryType, PrefabName));
		PropertyFragment = *Prefab;
	}
}

FName UPropertyFragment_MeleeWeapon::GetPropertyTagName()
{
	return FName("InventorySystem.Property.MeleeWeapon");
}

FName UPropertyFragment_MeleeWeapon::GetRegistryTypeName()
{
	return FName("MeleeWeaponRegistry");
}

void UPropertyFragment_MeleeWeapon::OnWeaponPutOn()
{
	UPropertyFragment_EntityLink* EntityLink = Owner->FindPropertyFragment<UPropertyFragment_EntityLink>();
	UMeshComponent* Mesh = nullptr;
	if (EntityLink && EntityLink->GetEntity())
	{
		HitTraceComponent = EntityLink->GetEntity()->GetComponentByClass<UHitTraceComponent>();
		TrailingComponent = EntityLink->GetEntity()->GetComponentByClass<UTrailingComponent>();
		Mesh = EntityLink->GetEntity()->GetComponentByClass<UMeshComponent>();
	}

	if (HitTraceComponent != nullptr && Mesh != nullptr)
	{
		auto OwnerActor = Owner->BelongingInventory->GetOwner();
		HitTraceComponent->Setup(Mesh);
		HitTraceComponent->ActorsToIgnore.AddUnique(OwnerActor);
		HitTraceComponent->UniqueHitDelegate.AddDynamic(this, &ThisClass::OnWeaponHit);
	}

	if (TrailingComponent != nullptr && Mesh != nullptr)
	{
		TrailingComponent->RibbonTrailingSetup(Mesh);
	}
}

void UPropertyFragment_MeleeWeapon::OnWeaponTakeOff()
{
	if (AbilitySystemComponent && HitTraceComponent)
	{
		HitTraceComponent->ActorsToIgnore.Remove(AbilitySystemComponent->GetOwner());
		HitTraceComponent->Teardown();
		HitTraceComponent->UniqueHitDelegate.RemoveAll(this);
		AbilitySystemComponent = nullptr;
		HitTraceComponent = nullptr;
	}

	if (TrailingComponent)
	{
		TrailingComponent->Teardown();
		TrailingComponent = nullptr;
	}
}

void UPropertyFragment_MeleeWeapon::OnWeaponHit(FHitResult HitResult)
{
	AbilitySystemComponent->HandleHitEvent(FGameplayTag::RequestGameplayTag(FName("AncientBattlefield.Event.Hit.MeleeWeapon")), HitResult.GetActor());
}