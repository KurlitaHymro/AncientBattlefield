// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/RegisteredAbilitySystemComponent.h"
#include "CombatAbilitySystemComponent.generated.h"

/**
 * Provide new features, including DagameHandle.
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class ANCIENTBATTLEFIELD_API UCombatAbilitySystemComponent : public URegisteredAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UCombatAbilitySystemComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dagame")
	FGameplayTagContainer DamageDefault;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCombatStanceComponent* CombatStanceComponent;

public:
	UFUNCTION(BlueprintCallable)
	void HandleHitEvent(FGameplayTag EventTag, AActor* TargetActor);

private:
	static FName RegistryType;

	void HandleDamageDefault(UAbilitySystemComponent* TargetASC);
};
