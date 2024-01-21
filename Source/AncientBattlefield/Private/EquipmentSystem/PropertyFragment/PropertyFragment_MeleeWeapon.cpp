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

FGameplayTag UPropertyFragment_MeleeWeapon::PropertyTag(FGameplayTag::RequestGameplayTag(TEXT("InventorySystem.Property.MeleeWeapon")));
FName UPropertyFragment_MeleeWeapon::RegistryType(TEXT("MeleeWeaponRegistry"));
FGameplayTag UPropertyFragment_MeleeWeapon::HitEventTag(FGameplayTag::RequestGameplayTag(TEXT("AncientBattlefield.Event.Hit.MeleeWeapon")));

void UPropertyFragment_MeleeWeapon::Init(const FName Template)
{
	Super::Init(Template);

	auto EntityLink = Owner->FindPropertyFragment<UPropertyFragment_EntityLink>();
	EntityLink->ItemSpawnEntityDelegate.AddDynamic(this, &ThisClass::OnSpawnEntity);
}

void UPropertyFragment_MeleeWeapon::InitFromRegistry(FName Template)
{
	auto Registry = UDataRegistrySubsystem::Get()->GetRegistryForType(GetRegistryTypeName());
	if (Registry)
	{
		auto Prefab = Registry->GetCachedItem<FPropertyFragmentMeleeWeapon>(FDataRegistryId(GetRegistryTypeName(), Template));
		PropertyFragment = *Prefab;
	}
}

FGameplayTag UPropertyFragment_MeleeWeapon::GetPropertyTag()
{
	return PropertyTag;
}

FName UPropertyFragment_MeleeWeapon::GetRegistryTypeName()
{
	return RegistryType;
}

FGameplayTagContainer UPropertyFragment_MeleeWeapon::GetRequiredTags()
{
	auto Tags = Super::GetRequiredTags();
	Tags.AddTag(UPropertyFragment_Equipment::PropertyTag);
	return Tags;
}

void UPropertyFragment_MeleeWeapon::OnSpawnEntity(AActor* Entity)
{
	OnWeaponPutOn();
}

void UPropertyFragment_MeleeWeapon::OnWeaponHit(FHitResult HitResult)
{
	AbilitySystemComponent->HandleHitEvent(HitEventTag, HitResult.GetActor());
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