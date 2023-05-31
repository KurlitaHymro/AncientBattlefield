// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AbilitiesInputComponent.h"
#include "Components/RegisteredAbilitySystemComponent.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystemGlobals.h"

void UAbilitiesInputComponent::SetupBinding(UInputAction* InputAction, int32 AbilityID)
{
	if (!ASC)
	{
		EnableBindings();
	}

	FAbilityInputBinding* AbilityInputBinding = MappedAbilities.Find(InputAction);
	if (AbilityInputBinding)
	{
		ASC->DisableAbility(AbilityInputBinding->AbilitesStack.Top());
	}
	else
	{
		AbilityInputBinding = &MappedAbilities.Add(InputAction);
	}

	AbilityInputBinding->AbilitesStack.Push(AbilityID);

	// Bind to InputAction
	if (InputComponent)
	{
		// Pressed event
		if (AbilityInputBinding->OnPressedHandle == 0)
		{
			AbilityInputBinding->OnPressedHandle = InputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::OnAbilityInputPressed, InputAction).GetHandle();
		}

		// Released event
		if (AbilityInputBinding->OnReleasedHandle == 0)
		{
			AbilityInputBinding->OnReleasedHandle = InputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &ThisClass::OnAbilityInputReleased, InputAction).GetHandle();
		}
	}
}

void UAbilitiesInputComponent::TeardownAbilityBinding(int32 AbilityID)
{
	ensure(ASC);

	// Find the mapping for this ability
	auto MappedIterator = MappedAbilities.CreateIterator();
	while (MappedIterator)
	{
		if (MappedIterator.Value().AbilitesStack.Top() == AbilityID)
		{
			break;
		}

		++MappedIterator;
	}

	if (MappedIterator)
	{
		FAbilityInputBinding& AbilityInputBinding = MappedIterator.Value();

		if (AbilityInputBinding.AbilitesStack.Remove(AbilityID) > 0)
		{
			if (AbilityInputBinding.AbilitesStack.Num() > 0)
			{
				ASC->EnableAbility(AbilityID);
			}
			else
			{
				TeardownActionBinding(MappedIterator.Key());
			}
		}
	}
}

void UAbilitiesInputComponent::TeardownActionBinding(UInputAction* InputAction)
{
	ensure(ASC);

	if (FAbilityInputBinding* Binding = MappedAbilities.Find(InputAction))
	{
		if (InputComponent)
		{
			InputComponent->RemoveBindingByHandle(Binding->OnPressedHandle);
			InputComponent->RemoveBindingByHandle(Binding->OnReleasedHandle);
		}

		for (auto AbilityID : Binding->AbilitesStack)
		{
			ASC->DisableAbility(AbilityID);
		}

		MappedAbilities.Remove(InputAction);
	}
}

void UAbilitiesInputComponent::SetupPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent)
{
	DisableBindings();

	for (auto& Ability : MappedAbilities)
	{
		UInputAction* InputAction = Ability.Key;

		// Pressed event
		InputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::OnAbilityInputPressed, InputAction);

		// Released event
		InputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &ThisClass::OnAbilityInputReleased, InputAction);
	}

	EnableBindings();
}

void UAbilitiesInputComponent::TeardownPlayerControls_Implementation(UEnhancedInputComponent* PlayerInputComponent)
{
	DisableBindings();
}

void UAbilitiesInputComponent::EnableBindings()
{
	AActor* MyOwner = GetOwner();
	check(MyOwner);

	ASC = Cast<URegisteredAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(MyOwner));
	if (ASC)
	{
		for (auto& InputBinding : MappedAbilities)
		{
			for (auto AbilityID : InputBinding.Value.AbilitesStack)
			{
				ASC->EnableAbility(AbilityID);
			}
		}
	}
}

void UAbilitiesInputComponent::DisableBindings()
{
	for (auto& InputBinding : MappedAbilities)
	{
		if (InputComponent)
		{
			InputComponent->RemoveBindingByHandle(InputBinding.Value.OnPressedHandle);
			InputComponent->RemoveBindingByHandle(InputBinding.Value.OnReleasedHandle);
		}

		if (ASC)
		{
			for (auto AbilityID : InputBinding.Value.AbilitesStack)
			{
				ASC->DisableAbility(AbilityID);
			}
		}
	}

	ASC = nullptr;
}

void UAbilitiesInputComponent::OnAbilityInputPressed(UInputAction* InputAction)
{
	if (ASC)
	{
		FAbilityInputBinding* FoundBinding = MappedAbilities.Find(InputAction);
		if (FoundBinding)
		{
			ASC->AbilityLocalInputPressed(FoundBinding->AbilitesStack.Top());
		}
	}
}

void UAbilitiesInputComponent::OnAbilityInputReleased(UInputAction* InputAction)
{
	if (ASC)
	{
		FAbilityInputBinding* FoundBinding = MappedAbilities.Find(InputAction);
		if (FoundBinding)
		{
			ASC->AbilityLocalInputReleased(FoundBinding->AbilitesStack.Top());
		}
	}
}
