// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/NotifyState/AnimNotifyState_HitTrace.h"
#include "CombatCore/CombatCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

void UAnimNotifyState_HitTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	OwnerCharacter = Cast<ACombatCharacter>(MeshComp->GetOwner());
	LastHitPointLocation = MeshComp->GetSocketLocation(FName("Weapon_R"));

	HitActors.Empty();
}

void UAnimNotifyState_HitTrace::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	FVector CurrentHitPointLocation = MeshComp->GetSocketLocation(FName("Weapon_R"));
	if (CurrentHitPointLocation.IsNearlyZero() || CurrentHitPointLocation.Equals(LastHitPointLocation))
	{
		return;
	}

	/* µ¶¹â¼ì²â */
	TArray<FHitResult> TickAllHitResult;
	TArray<AActor*> HitTargetIgnore;
	HitTargetIgnore.Add(MeshComp->GetOwner());
	UKismetSystemLibrary::BoxTraceMulti(MeshComp->GetOwner()->GetWorld(),
		LastHitPointLocation,
		CurrentHitPointLocation,
		FVector(4.f, 6.f, 12.f), // ÃÜ¶È£¬ÈÐ¿í£¬ÈÐ³¤
		MeshComp->GetComponentRotation(), //Weapon->GetMesh()->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery4,
		false,
		HitTargetIgnore,
		EDrawDebugTrace::ForDuration,
		TickAllHitResult,
		true);
	LastHitPointLocation = CurrentHitPointLocation;

	for (auto HitResult : TickAllHitResult)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActors.Contains(HitActor))
		{
			continue;
		}
		else
		{
			HitActors.Emplace(HitActor);
			OwnerCharacter->OnHitActor.Broadcast(HitActor);
		}
	}
}
