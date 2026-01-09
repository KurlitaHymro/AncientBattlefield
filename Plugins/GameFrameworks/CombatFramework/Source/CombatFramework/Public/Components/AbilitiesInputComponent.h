// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnInputComponent.h"
#include "GameplayTagsManager.h"
#include "AbilitiesInputComponent.generated.h"

USTRUCT()
struct FInputActionEventHandle
{
	GENERATED_BODY()

	uint32 OnTriggeredHandle = 0;
	uint32 OnStartedHandle = 0;
	uint32 OnOngoingHandle = 0;
	uint32 OnCanceledHandle = 0;
	uint32 OnCompletedHandle = 0;
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
	void SetupBinding(class UProfessionComponent* ProfessionComponent, UInputAction* InputAction);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void TeardownBinding(UInputAction* InputAction);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSet<class UProfessionComponent*> ProfessionComponents;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class URegisteredAbilitySystemComponent* RegisteredAbilitySystemComponent;

protected:
	/** Native/BP Event to set up player controls */
	virtual void SetupPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent) override;

	/** Native/BP Event to undo control setup */
	virtual void TeardownPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent) override;

private:
	void OnInputTriggered(const FInputActionValue& InputActionValue, UInputAction* InputAction);
	void OnInputStarted(const FInputActionValue& InputActionValue, UInputAction* InputAction);
	void OnInputOngoing(const FInputActionValue& InputActionValue, UInputAction* InputAction);
	void OnInputCanceled(const FInputActionValue& InputActionValue, UInputAction* InputAction);
	void OnInputCompleted(const FInputActionValue& InputActionValue, UInputAction* InputAction);

private:
	UPROPERTY(transient)
	TMap<UInputAction*, FInputActionEventHandle> InputActionEvents;
};
