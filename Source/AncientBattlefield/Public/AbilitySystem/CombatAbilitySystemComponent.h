// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/RegisteredAbilitySystemComponent.h"
#include "CombatAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class ANCIENTBATTLEFIELD_API UCombatAbilitySystemComponent : public URegisteredAbilitySystemComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UFUNCTION(BlueprintCallable)
	void HandleHitEvent(AActor* Target);

private:
	class ACombatCharacter* OwnerCharacter;
};
