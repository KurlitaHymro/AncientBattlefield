// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AttackAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class ANCIENTBATTLEFIELD_API UAttackAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UAttackAttributeSet, AttackPower)
	GAMEPLAYATTRIBUTE_VALUE_GETTER(AttackPower)
	GAMEPLAYATTRIBUTE_VALUE_SETTER(AttackPower)
	GAMEPLAYATTRIBUTE_VALUE_INITTER(AttackPower)

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UAttackAttributeSet, CauseDamageFactor)
	GAMEPLAYATTRIBUTE_VALUE_GETTER(CauseDamageFactor)
	GAMEPLAYATTRIBUTE_VALUE_SETTER(CauseDamageFactor)
	GAMEPLAYATTRIBUTE_VALUE_INITTER(CauseDamageFactor)

	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UAttackAttributeSet, ShockLevel)
	GAMEPLAYATTRIBUTE_VALUE_GETTER(ShockLevel)
	GAMEPLAYATTRIBUTE_VALUE_SETTER(ShockLevel)
	GAMEPLAYATTRIBUTE_VALUE_INITTER(ShockLevel)

private:
	friend struct DamageStatics;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackPower;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CauseDamageFactor;

	UPROPERTY(BlueprintReadOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
	FGameplayAttributeData ShockLevel;
};