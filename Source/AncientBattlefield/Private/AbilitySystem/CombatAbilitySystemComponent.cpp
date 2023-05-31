// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/CombatAbilitySystemComponent.h"
#include "CombatCore/CombatCharacter.h"
#include "AbilitySystemGlobals.h"

void UCombatAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACombatCharacter>(GetOwner());
	OwnerCharacter->OnHitActor.AddDynamic(this, &ThisClass::HandleHitEvent);
}

void UCombatAbilitySystemComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	OwnerCharacter->OnHitActor.RemoveAll(this);
}

void UCombatAbilitySystemComponent::HandleHitEvent(AActor* Target)
{
	AActor* Instigator = GetOwner();
	auto TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Target);
	if (TargetASC != nullptr && IsValid(Instigator) && IsValid(Target))
	{
		FGameplayEventData Payload;
		Payload.Instigator = Instigator;
		Payload.Target = Target;
	
		//FVector DamageSourceDirection = Instigator->GetActorLocation() - Target->GetActorLocation();
		//FVector ForwardVector = Target->GetActorForwardVector();
		//const float AngleDifference = DamageSourceDirection.CosineAngle2D(ForwardVector);
		//const float ShieldDefauseThresholdCosineAngle = 0.5f;
		//if (AngleDifference > ShieldDefauseThresholdCosineAngle && TargetASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Battle.Base.Status.ShieldDefense"))))
		//{
		//	HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("Battle.Base.Event.HitShieldDefense")), &Payload);
		//}
		//else
		//{
		//	HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("Battle.Base.Event.HitDefault")), &Payload);
		//}
		HandleGameplayEvent(FGameplayTag::RequestGameplayTag(FName("AncientBattlefield.Event.Hit.Deafult")), &Payload);
	}
}