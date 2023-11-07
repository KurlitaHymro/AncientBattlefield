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
		ASC->DisableAbility(AbilityInputBinding->AbilityID);
	}
	else
	{
		AbilityInputBinding = &MappedAbilities.Add(InputAction);

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
	ASC->EnableAbility(AbilityID);
	AbilityInputBinding->AbilityID = AbilityID;
}

void UAbilitiesInputComponent::TeardownAbilityBinding(int32 AbilityID)
{
	ensure(ASC);

	// Find the mapping for this ability
	auto MappedIterator = MappedAbilities.CreateIterator();
	while (MappedIterator)
	{
		if (MappedIterator.Value().AbilityID == AbilityID)
		{
			break;
		}

		++MappedIterator;
	}

	if (MappedIterator)
	{
		FAbilityInputBinding& AbilityInputBinding = MappedIterator.Value();

		ASC->DisableAbility(AbilityInputBinding.AbilityID);
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
		ASC->DisableAbility(Binding->AbilityID);
		MappedAbilities.Remove(InputAction);
	}
}

int32 UAbilitiesInputComponent::FindMappedAbilitiy(UInputAction* InputAction)
{
	auto Binding = MappedAbilities.Find(InputAction);
	if (Binding)
	{
		return Binding->AbilityID;
	}
	return 0;
}

void UAbilitiesInputComponent::TryPress(UInputAction* InputAction)
{
	bool* Status = ActionStatus.Find(InputAction);
	if (!Status)
	{
		Status = &ActionStatus.Add(InputAction);
	}

	if (!(*Status))
	{
		*Status = true;
		if (ASC)
		{
			FAbilityInputBinding* FoundBinding = MappedAbilities.Find(InputAction);
			if (FoundBinding && FoundBinding->AbilityID)
			{
				ASC->AbilityLocalInputPressed(FoundBinding->AbilityID);
				PressedDelegate.Broadcast(InputAction);
			}
		}
	}
}

void UAbilitiesInputComponent::TryReleased(UInputAction* InputAction)
{
	bool* Status = ActionStatus.Find(InputAction);
	if (Status && *Status)
	{
		*Status = false;
		if (ASC)
		{
			FAbilityInputBinding* FoundBinding = MappedAbilities.Find(InputAction);
			if (FoundBinding && FoundBinding->AbilityID)
			{
				ASC->AbilityLocalInputReleased(FoundBinding->AbilityID);
				ReleasedDelegate.Broadcast(InputAction);
			}
		}
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
			ASC->EnableAbility(InputBinding.Value.AbilityID);
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
			ASC->DisableAbility(InputBinding.Value.AbilityID);
		}
	}

	ASC = nullptr;
}

void UAbilitiesInputComponent::OnAbilityInputPressed(UInputAction* InputAction)
{
	TryPress(InputAction);
}

void UAbilitiesInputComponent::OnAbilityInputReleased(UInputAction* InputAction)
{
	TryReleased(InputAction);
}
