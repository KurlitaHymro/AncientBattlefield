// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatCore/MovementInputComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UMovementInputComponent::SetupPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerControls_Implementation(PlayerInputComponent);

	BindInputAction(MoveInputAction, ETriggerEvent::Triggered, this, &ThisClass::HandleMove);
	BindInputAction(LookInputAction, ETriggerEvent::Triggered, this, &ThisClass::HandleLook);
}

void UMovementInputComponent::HandleMove(const FInputActionValue& InputActionValue)
{
	APawn* MyPawn = GetPawnChecked<APawn>();
	FVector Val = InputActionValue.Get<FInputActionValue::Axis3D>();

	if (MyPawn->Controller == nullptr || Val.IsZero())
	{
		return;
	}
	if (Val.Size() < 0.65f) {
		Val = UKismetMathLibrary::ClampVectorSize(Val, 0.3f, 0.5f);
	}
	else {
		Val = UKismetMathLibrary::ClampVectorSize(Val, 0.8f, 1.0f);
	}

	// find out which way is right
	const FRotator Rotation = MyPawn->Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get right vector 
	const FVector DirectionX = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	const FVector DirectionY = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// add movement in that direction
	MyPawn->AddMovementInput(DirectionX, Val.X);
	MyPawn->AddMovementInput(DirectionY, Val.Y);
}

void UMovementInputComponent::HandleLook(const FInputActionValue& InputActionValue)
{
	APawn* MyPawn = GetPawnChecked<APawn>();
	FVector2D Val = InputActionValue.Get<FInputActionValue::Axis2D>();

	if (MyPawn->Controller == nullptr || Val.IsZero())
	{
		return;
	}

	// find out which way is right
	const FRotator Rotation = MyPawn->Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get right vector 
	const FVector DirectionY = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	const FVector DirectionX = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// add movement in that direction
	MyPawn->AddControllerPitchInput(Val.Y * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	MyPawn->AddControllerYawInput(Val.X * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
