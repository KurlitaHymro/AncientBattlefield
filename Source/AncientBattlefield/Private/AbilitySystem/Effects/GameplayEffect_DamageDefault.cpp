// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Effects/GameplayEffect_DamageDefault.h"
#include "AbilitySystem/Calculations/DamageExecutionCalculation.h"

UGameplayEffect_DamageDefault::UGameplayEffect_DamageDefault()
{
	FGameplayEffectExecutionDefinition ExecutionDefinition;
	ExecutionDefinition.CalculationClass = UDamageExecutionCalculation::StaticClass();
	Executions.Add(ExecutionDefinition);
}