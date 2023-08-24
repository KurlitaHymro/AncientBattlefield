// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnInputComponent.h"
#include "AbilitiesInputComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilitiesInputPressedDelegate, class UInputAction*, Action);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilitiesInputReleasedDelegate, class UInputAction*, Action);

USTRUCT()
struct FAbilityInputBinding
{
	GENERATED_BODY()

	uint32 OnPressedHandle = 0;
	uint32 OnReleasedHandle = 0;
	int32 AbilityID;
};

/**
 * 
 */
UCLASS()
class COMBATFRAMEWORK_API UAbilitiesInputComponent : public UPawnInputComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void SetupBinding(UInputAction* InputAction, int32 AbilityID);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void TeardownAbilityBinding(int32 AbilityID);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void TeardownActionBinding(UInputAction* InputAction);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	int32 FindMappedAbilitiy(UInputAction* InputAction);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void TryPress(UInputAction* InputAction);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void TryReleased(UInputAction* InputAction);

public:
	UPROPERTY(BlueprintAssignable)
	FAbilitiesInputPressedDelegate PressedDelegate;

	UPROPERTY(BlueprintAssignable)
	FAbilitiesInputReleasedDelegate ReleasedDelegate;

protected:
	/** Native/BP Event to set up player controls */
	virtual void SetupPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent) override;

	/** Native/BP Event to undo control setup */
	virtual void TeardownPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent) override;

private:
	void EnableBindings();
	void DisableBindings();
	void OnAbilityInputPressed(UInputAction* InputAction);
	void OnAbilityInputReleased(UInputAction* InputAction);

private:
	UPROPERTY(transient)
	class URegisteredAbilitySystemComponent* ASC;

	UPROPERTY(transient)
	TMap<UInputAction*, FAbilityInputBinding> MappedAbilities;

	UPROPERTY(transient)
	TMap<UInputAction*, bool> ActionStatus;
};
