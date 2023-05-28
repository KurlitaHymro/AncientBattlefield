// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CRCharacter.h"
#include "AbilitySystemInterface.h"
#include "CombatCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHitActorDelegate, AActor*, Target);

/**
 * Combat Gameplay Core Character
 * 负责各项战斗相关模块（诸如技能系统、装备系统、伤害系统等）之间的功能配合。
 */
UCLASS()
class ANCIENTBATTLEFIELD_API ACombatCharacter : public ACRCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACombatCharacter();

protected:
	// APawn interface Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called PreInitializeComponents
	virtual void PreInitializeComponents() override;

	// To add mapping context
	virtual void BeginPlay();

public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	class UAbilitySystemComponent* AbilitySystemComponent;

public:
	UPROPERTY(BlueprintAssignable)
	FHitActorDelegate OnHitActor;
};
