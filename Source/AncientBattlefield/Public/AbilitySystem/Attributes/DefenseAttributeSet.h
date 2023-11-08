// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "DefenseAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class ANCIENTBATTLEFIELD_API UDefenseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UDefenseAttributeSet, DefensePower)
	GAMEPLAYATTRIBUTE_VALUE_GETTER(DefensePower)
	GAMEPLAYATTRIBUTE_VALUE_SETTER(DefensePower)
	GAMEPLAYATTRIBUTE_VALUE_INITTER(DefensePower)

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UDefenseAttributeSet, ApplyDamageFactor)
	GAMEPLAYATTRIBUTE_VALUE_GETTER(ApplyDamageFactor)
	GAMEPLAYATTRIBUTE_VALUE_SETTER(ApplyDamageFactor)
	GAMEPLAYATTRIBUTE_VALUE_INITTER(ApplyDamageFactor)

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UDefenseAttributeSet, StoicLevel)
	GAMEPLAYATTRIBUTE_VALUE_GETTER(StoicLevel)
	GAMEPLAYATTRIBUTE_VALUE_SETTER(StoicLevel)
	GAMEPLAYATTRIBUTE_VALUE_INITTER(StoicLevel)

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UDefenseAttributeSet, StableState)
	GAMEPLAYATTRIBUTE_VALUE_GETTER(StableState)
	GAMEPLAYATTRIBUTE_VALUE_SETTER(StableState)
	GAMEPLAYATTRIBUTE_VALUE_INITTER(StableState)

protected:
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

private:
	friend struct DamageStatics;

	UPROPERTY(BlueprintReadOnly, Category = "Defense", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData DefensePower;

	UPROPERTY(BlueprintReadOnly, Category = "Defense", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ApplyDamageFactor;

	UPROPERTY(BlueprintReadOnly, Category = "Defense", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData StoicLevel;

	UPROPERTY(BlueprintReadOnly, Category = "Defense", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData StableState;
};
