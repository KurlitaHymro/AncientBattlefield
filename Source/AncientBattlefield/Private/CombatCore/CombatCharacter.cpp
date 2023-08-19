// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatCore/CombatCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/AbilitiesInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/CombatAbilitySystemComponent.h"
#include "InventoryComponent.h"
#include "EquipmentSystem/EquipmentComponent.h"

ACombatCharacter::ACombatCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(50.f, 88.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 420.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = 420.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	CombatAbilityComponent = CreateDefaultSubobject<UCombatAbilitySystemComponent>(TEXT("CombatAbilitySystemComponent"));
	CombatAbilityComponent->SetIsReplicated(true);

	AbilitiesInputComponent = CreateDefaultSubobject<UAbilitiesInputComponent>(TEXT("AbilitiesInputComponent"));

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("Equipment"));
}

void ACombatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACombatCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}

void ACombatCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (CombatAbilityComponent && InventoryComponent && EquipmentComponent)
	{
		bIsActive = true;
		CombatAbilityComponent->SwitchBodyForm(DefaultBodyForm);
	}
}

UAbilitySystemComponent* ACombatCharacter::GetAbilitySystemComponent() const
{
	return CombatAbilityComponent;
}

UInventoryComponent* ACombatCharacter::GetInventorySystemComponent() const
{
	return InventoryComponent;
}

UEquipmentComponent* ACombatCharacter::GetEquipmentSystemComponent() const
{
	return EquipmentComponent;
}

void ACombatCharacter::Die_Implementation()
{
	CombatCharacterDieDelegate.Broadcast();

	for (auto i = 0; i < EquipmentComponent->GetSize(); i++)
	{
		auto Equipment = EquipmentComponent->GetEquipment((EEquipmentSlots)i);
		if (Equipment)
		{
			EquipmentComponent->RemoveItem(Equipment);
		}
	}

	if (!AnimConfig.DieAnim.IsEmpty())
	{
		auto Anim = AnimConfig.DieAnim[FMath::Rand() % AnimConfig.DieAnim.Num()];
		PlayAnimMontage(Anim);
	}

	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, this, &ThisClass::OnDestroy, 5.0f, false);

	DetachFromControllerPendingDestroy();
	bIsActive = false;
}

void ACombatCharacter::Unstable_Implementation(float StableState)
{
	if (!bIsActive)
	{
		return;
	}
	UAnimMontage* Anim = nullptr;
	float RootMotionOffset;
	float FindMagnitude = 0;
	for (auto UnstableLevel : AnimConfig.UnstableAnim)
	{
		if (StableState < FindMagnitude && StableState <= UnstableLevel.MagnitudeThreshold)
		{
			FindMagnitude = UnstableLevel.MagnitudeThreshold;
			Anim = UnstableLevel.Montage;
			RootMotionOffset = UnstableLevel.AnimRootMotionOffset;
		}
	}
	if (Anim)
	{
		PlayAnimMontage(Anim);
	}
}

void ACombatCharacter::OnDestroy_Implementation()
{
	Destroy();
}
