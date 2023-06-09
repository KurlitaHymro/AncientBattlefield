// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RegisteredAbilitySystemComponent.generated.h"

USTRUCT(BlueprintType)
struct FBattleAbilityEntry
{
	GENERATED_BODY()

	// SoftClass of Ability to grant
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<UGameplayAbility> AbilityType;

	UPROPERTY(BlueprintReadOnly)
	FGameplayAbilitySpec AbilitySpec;

	UPROPERTY(BlueprintReadOnly)
	FGameplayAbilitySpecHandle AbilitySpecHandle;
};

/**
 * 
 */
UCLASS()
class COMBATFRAMEWORK_API URegisteredAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void EnableAbility(int32 AbilityID);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void DisableAbility(int32 AbilityID);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void RemoveAbility(int32 AbilityID);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	int32 LoadAbilityFromType(TSoftClassPtr<UGameplayAbility> AbilityType);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	int32 FindAbilityByType(TSoftClassPtr<UGameplayAbility> AbilityType);

private:
	UPROPERTY(transient)
	TMap<int32, FBattleAbilityEntry> MappedAbilities;
};
