// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagAssetInterface.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AnimNotify_SendGameplayEvent.generated.h"

/**
 * 
 */
UCLASS()
class ANCIENTBATTLEFIELD_API UAnimNotify_SendGameplayEvent : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_SendGameplayEvent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))
	FGameplayEventData Payload;

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
