// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnInputComponent.h"
#include "MovementInputComponent.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class ANCIENTBATTLEFIELD_API UMovementInputComponent : public UPawnInputComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float BaseLookUpRate = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float BaseTurnRate = 50.f;

protected:
	virtual void SetupPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent) override;

	/** Movement input handlers */
	void HandleMove(const FInputActionValue& InputActionValue);
	void HandleSight(const FInputActionValue& InputActionValue);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SightInputAction;
};
