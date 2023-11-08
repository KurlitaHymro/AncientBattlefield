// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Calculations/DamageExecutionCalculation.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AttackAttributeSet)
#include UE_INLINE_GENERATED_CPP_BY_NAME(DefenseAttributeSet)
#include UE_INLINE_GENERATED_CPP_BY_NAME(HealthAttributeSet)

// -------------------------------------------------------------------------
//	Helper macros for declaring attribute captures 
// -------------------------------------------------------------------------

#define DECLARE_SOURCE_ATTRIBUTE_CAPTUREDEF(P, T) \
	FProperty* T##P##Property; \
	FGameplayEffectAttributeCaptureDefinition T##P##Def; \

#define DEFINE_SOURCE_ATTRIBUTE_CAPTUREDEF(S, P, T, B) \
{ \
	T##P##Property = FindFieldChecked<FProperty>(S::StaticClass(), GET_MEMBER_NAME_CHECKED(S, P)); \
	T##P##Def = FGameplayEffectAttributeCaptureDefinition(T##P##Property, EGameplayEffectAttributeCaptureSource::T, B); \
}

struct DamageStatics
{
	DECLARE_SOURCE_ATTRIBUTE_CAPTUREDEF(AttackPower, Source);
	DECLARE_SOURCE_ATTRIBUTE_CAPTUREDEF(CauseDamageFactor, Source);
	DECLARE_SOURCE_ATTRIBUTE_CAPTUREDEF(ShockLevel, Source);

	DECLARE_SOURCE_ATTRIBUTE_CAPTUREDEF(DefensePower, Target);
	DECLARE_SOURCE_ATTRIBUTE_CAPTUREDEF(ApplyDamageFactor, Target);
	DECLARE_SOURCE_ATTRIBUTE_CAPTUREDEF(StoicLevel, Target);
	DECLARE_SOURCE_ATTRIBUTE_CAPTUREDEF(StableState, Target);

	DECLARE_SOURCE_ATTRIBUTE_CAPTUREDEF(Health, Target);

	DamageStatics()
	{
		// �˺���Դ������
		DEFINE_SOURCE_ATTRIBUTE_CAPTUREDEF(UAttackAttributeSet, AttackPower, Source, true);
		DEFINE_SOURCE_ATTRIBUTE_CAPTUREDEF(UAttackAttributeSet, CauseDamageFactor, Source, true);
		DEFINE_SOURCE_ATTRIBUTE_CAPTUREDEF(UAttackAttributeSet, ShockLevel, Source, true);

		// �˺�Ŀ�������
		DEFINE_SOURCE_ATTRIBUTE_CAPTUREDEF(UDefenseAttributeSet, DefensePower, Target, true);
		DEFINE_SOURCE_ATTRIBUTE_CAPTUREDEF(UDefenseAttributeSet, ApplyDamageFactor, Target, true);
		DEFINE_SOURCE_ATTRIBUTE_CAPTUREDEF(UDefenseAttributeSet, StoicLevel, Target, true);
		DEFINE_SOURCE_ATTRIBUTE_CAPTUREDEF(UDefenseAttributeSet, StableState, Target, true);

		DEFINE_SOURCE_ATTRIBUTE_CAPTUREDEF(UHealthAttributeSet, Health, Target, true);
	}
};

static const DamageStatics& DamageStaticsInst()
{
	static DamageStatics DamageStatics;
	return DamageStatics;
}

UDamageExecutionCalculation::UDamageExecutionCalculation()
{
	RelevantAttributesToCapture.Add(DamageStaticsInst().SourceAttackPowerDef);
	RelevantAttributesToCapture.Add(DamageStaticsInst().SourceCauseDamageFactorDef);
	RelevantAttributesToCapture.Add(DamageStaticsInst().SourceShockLevelDef);
	RelevantAttributesToCapture.Add(DamageStaticsInst().TargetDefensePowerDef);
	RelevantAttributesToCapture.Add(DamageStaticsInst().TargetApplyDamageFactorDef);
	RelevantAttributesToCapture.Add(DamageStaticsInst().TargetStoicLevelDef);
	RelevantAttributesToCapture.Add(DamageStaticsInst().TargetStableStateDef);
	RelevantAttributesToCapture.Add(DamageStaticsInst().TargetHealthDef);
}

void UDamageExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor_Direct() : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor_Direct() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// --------------------------------------
	//	CauseDamage = CauseDamageFactor * AttackPower
	//	ApplyDamage = CauseDamage * ApplyDamageFactor * (DefensePower / (DefensePower + 100))
	// --------------------------------------

	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().SourceAttackPowerDef, EvaluationParameters, SourceAttackPower);

	float SourceCauseDamageFactor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().SourceCauseDamageFactorDef, EvaluationParameters, SourceCauseDamageFactor);

	float TargetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().TargetDefensePowerDef, EvaluationParameters, TargetDefensePower);

	float TargetApplyDamageFactor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().TargetApplyDamageFactorDef, EvaluationParameters, TargetApplyDamageFactor);

	// Health = Health - (��Դ������ * ��Դ����˺�ϵ�� * Ŀ������˺�ϵ�� * (100 / (Ŀ������� + 100)))
	float ApplyDamage = SourceAttackPower * SourceCauseDamageFactor * TargetApplyDamageFactor * (100.f / (TargetDefensePower + 100.f));
	UE_LOG(LogTemp, Error, TEXT("Damage %f"), ApplyDamage);
	if (ApplyDamage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().TargetHealthProperty, EGameplayModOp::Additive, -ApplyDamage));
	}

	// --------------------------------------
	// StableState = Stoic - ShockLevel
	// --------------------------------------

	float SourceShockLevel = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().SourceShockLevelDef, EvaluationParameters, SourceShockLevel);

	float TargetStoicLevel = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().TargetStoicLevelDef, EvaluationParameters, TargetStoicLevel);

	float ApplyShock = SourceShockLevel - TargetStoicLevel;
	UE_LOG(LogTemp, Error, TEXT("Shock %f"), ApplyShock);
	if (ApplyShock > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().TargetStableStateProperty, EGameplayModOp::Additive, -ApplyShock));
	}
}