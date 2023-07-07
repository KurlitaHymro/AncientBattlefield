// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/NotifyState/AnimNotifyState_HitTrace.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CombatCore/CombatCharacter.h"
#include "Item/ItemObject.h"
#include "EquipmentSystem/PropertyFragment/PropertyFragment_Equipment.h"
#include "EquipmentSystem/PropertyFragment/PropertyFragment_MeleeWeapon.h"

void UAnimNotifyState_HitTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	OwnerCharacter = Cast<ACombatCharacter>(MeshComp->GetOwner());

	UItemObject* HitWeapon = OwnerCharacter->GetEquipmentSystemComponent()->GetItem((int32)EquipmentSlot);
	if (HitWeapon != nullptr)
	{
		UPropertyFragment_Equipment* HitWeaponInfo = HitWeapon->FindPropertyFragment<UPropertyFragment_Equipment>();
		UPropertyFragment_MeleeWeapon* HitPointInfo = HitWeapon->FindPropertyFragment<UPropertyFragment_MeleeWeapon>();
		if (HitWeaponInfo != nullptr && HitPointInfo != nullptr)
		{
			WeaponMesh = HitWeaponInfo->GetMesh();
			//HitPointLocation = WeaponMesh->GetSocketLocation(HitPoint);
			//HitPointHalfSize = *HitPointInfo->HitPoints.Find(HitPoint);
		}
	}

	HitActors.Empty();
}

void UAnimNotifyState_HitTrace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	FVector CurrentLocation;
	if (WeaponMesh != nullptr)
	{
		CurrentLocation = WeaponMesh->GetSocketLocation(HitPoint);
	}
	if (CurrentLocation.IsNearlyZero() || CurrentLocation.Equals(HitPointLocation))
	{
		return;
	}

	TArray<FHitResult> TickAllHitResult;
	TArray<AActor*> HitTargetIgnore;
	HitTargetIgnore.Add(MeshComp->GetOwner());

	/* µ¶¹â¼ì²â */
	UKismetSystemLibrary::BoxTraceMulti(MeshComp->GetOwner()->GetWorld(),
		HitPointLocation,
		CurrentLocation,
		HitPointHalfSize,
		WeaponMesh->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery4,
		false,
		HitTargetIgnore,
		EDrawDebugTrace::ForDuration,
		TickAllHitResult,
		true);
	HitPointLocation = CurrentLocation;

	for (auto HitResult : TickAllHitResult)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActors.Contains(HitActor))
		{
			continue;
		}
		else
		{
			HitActors.Add(HitActor);
			OwnerCharacter->OnHitActor.Broadcast(HitActor);
		}
	}
}
