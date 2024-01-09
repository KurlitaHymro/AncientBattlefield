// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularCharacter.h"
#include "AbilitySystemInterface.h"
#include "InventorySystemInterface.h"
#include "CombatCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterBehaviorDelegate);

USTRUCT(BlueprintType)
struct FUnstableAnimConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MagnitudeThreshold;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* Montage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float AnimRootMotionOffset;
};

USTRUCT(BlueprintType)
struct FCombatCharacterAnimConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UAnimMontage*> DieAnim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FUnstableAnimConfig> UnstableAnim;
};


/**
 * Combat Gameplay Core Character
 * 负责各项战斗相关模块（诸如技能系统、装备系统、伤害系统等）之间的功能配合。
 */
UCLASS()
class ANCIENTBATTLEFIELD_API ACombatCharacter : public AModularCharacter, public IAbilitySystemInterface, public IInventorySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACombatCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	// APawn interface Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called PreInitializeComponents
	virtual void PreInitializeComponents() override;

	// To add mapping context
	virtual void BeginPlay();

public:
	UPROPERTY(BlueprintReadWrite, Category = "CombatGameplay")
	bool bIsActive = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = AnimSettings)
	FCombatCharacterAnimConfig AnimConfig;

	UPROPERTY(BlueprintAssignable)
	FCharacterBehaviorDelegate CombatCharacterDieDelegate;

public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual class UInventoryComponent* GetInventorySystemComponent() const override;

	virtual class UAbilitiesInputComponent* GetAbilitiesInputComponent() const;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCombatAbilitySystemComponent> CombatAbilitySystem;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAbilitiesInputComponent> AbilitiesInput;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInventoryComponent> Inventory;

public:
	UFUNCTION(BlueprintNativeEvent)
	void Die();
	virtual void Die_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void Unstable(float StableState);
	virtual void Unstable_Implementation(float StableState);

protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnDestroy();
	virtual void OnDestroy_Implementation();
};
