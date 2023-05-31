// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnInputComponent.h"
#include "AbilitiesInputComponent.generated.h"

USTRUCT()
struct FAbilityInputBinding
{
	GENERATED_BODY()

	uint32 OnPressedHandle = 0;
	uint32 OnReleasedHandle = 0;
	TArray<int32> AbilitesStack;
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
};
