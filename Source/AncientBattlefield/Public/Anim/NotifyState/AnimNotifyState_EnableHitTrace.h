// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animations/AnimNotifyState_SelfEffectBase.h"
#include "AnimNotifyState_EnableHitTrace.generated.h"

/**
 * 
 */
UCLASS()
class ANCIENTBATTLEFIELD_API UAnimNotifyState_EnableHitTrace : public UAnimNotifyState_SelfEffectBase
{
	GENERATED_BODY()

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	class ACombatCharacter* OwnerCharacter;

	class UHitTraceComponent* HitTraceComponent;

	class UTrailingComponent* TrailingComponent;

	TArray<AActor*> HitActors;
};
