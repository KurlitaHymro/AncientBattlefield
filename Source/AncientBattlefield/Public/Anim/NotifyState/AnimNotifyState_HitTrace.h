// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animations/AnimNotifyState_SelfEffectBase.h"
#include "EquipmentSystem/EquipmentComponent.h"
#include "AnimNotifyState_HitTrace.generated.h"

/**
 * 
 */
UCLASS()
class ANCIENTBATTLEFIELD_API UAnimNotifyState_HitTrace : public UAnimNotifyState_SelfEffectBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Weapon")
	EEquipmentSlots EquipmentSlot;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName HitPoint;

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

private:
	class ACombatCharacter* OwnerCharacter;

	UMeshComponent* WeaponMesh;

	FVector HitPointLocation;
	FVector HitPointHalfSize;

	TArray<AActor*> HitActors;
};
