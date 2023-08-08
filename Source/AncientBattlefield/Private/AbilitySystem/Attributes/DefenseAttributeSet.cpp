// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attributes/DefenseAttributeSet.h"
#include "CombatCore/CombatCharacter.h"

void UDefenseAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	if (Attribute == GetStableStateAttribute())
	{
		if (NewValue < 0)
		{
			ACombatCharacter* TargetCharacter = GetTypedOuter<ACombatCharacter>();
			TargetCharacter->Unstable(NewValue);
		}
		StableState.SetCurrentValue(StableState.GetBaseValue());
	}
}