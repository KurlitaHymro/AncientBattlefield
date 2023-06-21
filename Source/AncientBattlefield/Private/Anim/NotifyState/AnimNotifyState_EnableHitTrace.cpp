// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/NotifyState/AnimNotifyState_EnableHitTrace.h"
#include "CombatCore/CombatCharacter.h"
#include "Item/ItemObject.h"
#include "EquipmentSystem/EquipmentComponent.h"
#include "EquipmentSystem/PropertyFragment/PropertyFragment_Equipment.h"
#include "EquipmentSystem/PropertyFragment/PropertyFragment_MeleeWeapon.h"

void UAnimNotifyState_EnableHitTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	OwnerCharacter = Cast<ACombatCharacter>(MeshComp->GetOwner());
	if (OwnerCharacter)
	{
		auto EquipmentSystemComponent = OwnerCharacter->GetEquipmentSystemComponent();
		if (EquipmentSystemComponent)
		{
			auto Weapon = EquipmentSystemComponent->GetEquipment(EEquipmentSlots::MainHand);
			Weapon->FindPropertyFragment<UPropertyFragment_MeleeWeapon>();
		}
	}
}

void UAnimNotifyState_EnableHitTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}
