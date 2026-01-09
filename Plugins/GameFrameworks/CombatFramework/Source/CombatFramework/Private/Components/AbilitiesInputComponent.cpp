// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AbilitiesInputComponent.h"
#include "Components/RegisteredAbilitySystemComponent.h"
#include "Components/ProfessionComponent.h"
#include "CombatFrameworkTags.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemBlueprintLibrary.h"

void UAbilitiesInputComponent::SetupBinding(UProfessionComponent* ProfessionComponent, UInputAction* InputAction)
{
	ProfessionComponents.Add(ProfessionComponent);

	if (!InputActionEvents.Contains(InputAction))
	{
		FInputActionEventHandle Events;
		InputActionEvents.Add(InputAction, Events);

		// 职业组件可能更早尝试绑定输入，等待SetupPlayerControls中统一处理。
		if (InputComponent)
		{
			Events.OnTriggeredHandle = InputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::OnInputTriggered, InputAction).GetHandle();
			Events.OnStartedHandle = InputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::OnInputStarted, InputAction).GetHandle();
			Events.OnOngoingHandle = InputComponent->BindAction(InputAction, ETriggerEvent::Ongoing, this, &ThisClass::OnInputOngoing, InputAction).GetHandle();
			Events.OnCanceledHandle = InputComponent->BindAction(InputAction, ETriggerEvent::Canceled, this, &ThisClass::OnInputCanceled, InputAction).GetHandle();
			Events.OnCompletedHandle = InputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &ThisClass::OnInputCompleted, InputAction).GetHandle();
		}
	}
}

void UAbilitiesInputComponent::TeardownBinding(UInputAction* InputAction)
{
	if (FInputActionEventHandle* Events = InputActionEvents.Find(InputAction))
	{
		if (InputComponent && Events)
		{
			InputComponent->RemoveBindingByHandle(Events->OnTriggeredHandle);
			InputComponent->RemoveBindingByHandle(Events->OnStartedHandle);
			InputComponent->RemoveBindingByHandle(Events->OnOngoingHandle);
			InputComponent->RemoveBindingByHandle(Events->OnCanceledHandle);
			InputComponent->RemoveBindingByHandle(Events->OnCompletedHandle);
		}
	}
}

void UAbilitiesInputComponent::SetupPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent)
{
	RegisteredAbilitySystemComponent = Cast<URegisteredAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()));
	ensure(RegisteredAbilitySystemComponent);

	for (auto& Events : InputActionEvents)
	{
		UInputAction* InputAction = Events.Key;

		Events.Value.OnTriggeredHandle = InputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ThisClass::OnInputTriggered, InputAction).GetHandle();
		Events.Value.OnStartedHandle = InputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::OnInputStarted, InputAction).GetHandle();
		Events.Value.OnOngoingHandle = InputComponent->BindAction(InputAction, ETriggerEvent::Ongoing, this, &ThisClass::OnInputOngoing, InputAction).GetHandle();
		Events.Value.OnCanceledHandle = InputComponent->BindAction(InputAction, ETriggerEvent::Canceled, this, &ThisClass::OnInputCanceled, InputAction).GetHandle();
		Events.Value.OnCompletedHandle = InputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &ThisClass::OnInputCompleted, InputAction).GetHandle();
	}
}

void UAbilitiesInputComponent::TeardownPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent)
{
	if (InputComponent)
	{
		for (auto& Events : InputActionEvents)
		{
			InputComponent->RemoveBindingByHandle(Events.Value.OnTriggeredHandle);
			InputComponent->RemoveBindingByHandle(Events.Value.OnStartedHandle);
			InputComponent->RemoveBindingByHandle(Events.Value.OnOngoingHandle);
			InputComponent->RemoveBindingByHandle(Events.Value.OnCanceledHandle);
			InputComponent->RemoveBindingByHandle(Events.Value.OnCompletedHandle);
		}
		InputActionEvents.Empty();
	}

	for (UProfessionComponent* ProfessionComponent : ProfessionComponents)
	{
		ProfessionComponent->bIsActiveAbilitiesInput = false;
	}
	ProfessionComponents.Empty();
	RegisteredAbilitySystemComponent = nullptr;
}

void UAbilitiesInputComponent::OnInputTriggered(const FInputActionValue& InputActionValue, UInputAction* InputAction)
{
	for (UProfessionComponent* ProfessionComponent : ProfessionComponents)
	{
		if (ProfessionComponent->bIsActiveAbilitiesInput)
		{
			ProfessionComponent->ReportInput(InputAction, CombatFrameworkTags::Combat_Event_Input_Triggered, InputActionValue.Get<float>());
		}
	}
}

void UAbilitiesInputComponent::OnInputStarted(const FInputActionValue& InputActionValue, UInputAction* InputAction)
{
	for (UProfessionComponent* ProfessionComponent : ProfessionComponents)
	{
		if (ProfessionComponent->bIsActiveAbilitiesInput)
		{
			ProfessionComponent->ReportInput(InputAction, CombatFrameworkTags::Combat_Event_Input_Started, InputActionValue.Get<float>());
		}
	}
}

void UAbilitiesInputComponent::OnInputOngoing(const FInputActionValue& InputActionValue, UInputAction* InputAction)
{
	
}

void UAbilitiesInputComponent::OnInputCanceled(const FInputActionValue& InputActionValue, UInputAction* InputAction)
{
	for (UProfessionComponent* ProfessionComponent : ProfessionComponents)
	{
		if (ProfessionComponent->bIsActiveAbilitiesInput)
		{
			ProfessionComponent->ReportInput(InputAction, CombatFrameworkTags::Combat_Event_Input_Canceled, InputActionValue.Get<float>());
		}
	}
}

void UAbilitiesInputComponent::OnInputCompleted(const FInputActionValue& InputActionValue, UInputAction* InputAction)
{
	for (UProfessionComponent* ProfessionComponent : ProfessionComponents)
	{
		if (ProfessionComponent->bIsActiveAbilitiesInput)
		{
			ProfessionComponent->ReportInput(InputAction, CombatFrameworkTags::Combat_Event_Input_Completed, InputActionValue.Get<float>());
		}
	}
}