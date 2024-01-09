// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/NotifyState/AnimNotifyState_EnableHitTrace.h"
#include "CombatCore/CombatCharacter.h"
#include "Item/ItemObject.h"
#include "InventoryComponent.h"
#include "EquipmentSystem/PropertyFragment/PropertyFragment_Equipment.h"
#include "EquipmentSystem/PropertyFragment/PropertyFragment_MeleeWeapon.h"
#include "Anim/Components/HitTraceComponent.h"
#include "Anim/Components/TrailingComponent.h"

void UAnimNotifyState_EnableHitTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	OwnerCharacter = Cast<ACombatCharacter>(MeshComp->GetOwner());
	if (OwnerCharacter && OwnerCharacter->bIsActive)
	{
		auto InventorySystemComponent = OwnerCharacter->GetInventorySystemComponent();
		if (InventorySystemComponent)
		{
			auto Weapon = InventorySystemComponent->GetItem((int32)EEquipmentSlots::MainHand);
			if (Weapon)
			{
				auto MeleeWeaponProperty = Weapon->FindPropertyFragment<UPropertyFragment_MeleeWeapon>();
				if (MeleeWeaponProperty)
				{
					HitTraceComponent = MeleeWeaponProperty->GetHitTraceComponent();
					TrailingComponent = MeleeWeaponProperty->GetTrailingComponent();
				}
			}
		}
	}

	if (HitTraceComponent)
	{
		HitActors.Empty();
		HitTraceComponent->EnableTrace();
	}

	if (TrailingComponent)
	{
		TrailingComponent->EnableTrailing();
	}
} 

void UAnimNotifyState_EnableHitTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (HitTraceComponent)
	{
		HitTraceComponent->DisableTrace();
		HitTraceComponent = nullptr;
	}

	if (TrailingComponent)
	{
		TrailingComponent->DisableTrailing();
		TrailingComponent = nullptr;	
	}
}