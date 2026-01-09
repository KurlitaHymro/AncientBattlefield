// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ProfessionComponent.h"
#include "Components/AbilitiesInputComponent.h"
#include "Components/RegisteredAbilitySystemComponent.h"
#include "CombatFrameworkTags.h"
#include "AbilitySystemGlobals.h"

UProfessionComponent::UProfessionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bWantsInitializeComponent = true;
}

void UProfessionComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetupInputBinding();
}

int UProfessionComponent::AddAbility(TSoftClassPtr<class UGameplayAbility> AbilityType, FName Name, UTexture* Icon, UInputAction* InputAction)
{
	if (!RegisteredAbilitySystemComponent)
	{
		RegisteredAbilitySystemComponent = Cast<URegisteredAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()));
	}
	ensure(RegisteredAbilitySystemComponent);

	int32 AbilityID = RegisteredAbilitySystemComponent->FindAbilityByType(AbilityType);
	if (!AbilityID)
	{
		AbilityID = RegisteredAbilitySystemComponent->LoadAbilityFromType(AbilityType);
	}

	FAbilityInfo AbilityInfo;
	AbilityInfo.AbilityType = AbilityType;
	AbilityInfo.Name = Name;
	AbilityInfo.Icon = Icon;
	AbilitiesInfo.Add(AbilityID, AbilityInfo);

	if (InputAction)
	{
		MappedAbilities.Add(InputAction, AbilityID);

		// 如果已经建立输入绑定，则对新增技能追加绑定；否则先保存映射关系，建立输入绑定时再统一处理。
		if (bIsActiveAbilitiesInput && AbilitiesInputComponent)
		{
			AbilitiesInputComponent->SetupBinding(this, InputAction);
		}
	}
	return AbilityID;
}

void UProfessionComponent::RemoveAbility(int32 AbilityID)
{
	ensure(RegisteredAbilitySystemComponent && AbilitiesInputComponent);

	if (AbilitiesInfo.Contains(AbilityID))
	{
		AbilitiesInfo.Remove(AbilityID);

		for (auto Iterator = MappedAbilities.CreateIterator(); Iterator; ++Iterator)
		{
			if (Iterator.Value() == AbilityID)
			{
				Iterator.RemoveCurrent();
				break;
			}
		}
		RegisteredAbilitySystemComponent->RemoveAbility(AbilityID);
	}
}

int32 UProfessionComponent::QueryAbility(const UInputAction* InputAction) const
{
	const int32* AbilityID = MappedAbilities.Find(InputAction);
	if (AbilityID && *AbilityID)
	{
		return *AbilityID;
	}
	return 0;
}

FAbilityInfo UProfessionComponent::GetAbilityInfo(int32 AbilityID) const
{
	const FAbilityInfo* Info = AbilitiesInfo.Find(AbilityID);
	if (Info)
	{
		return *Info;
	}
	return FAbilityInfo();
}

void UProfessionComponent::SetupInputBinding()
{
	if (!AbilitiesInputComponent)
	{
		AbilitiesInputComponent = GetOwner()->FindComponentByClass<UAbilitiesInputComponent>();
	}

	if (AbilitiesInputComponent)
	{
		bIsActiveAbilitiesInput = true;

		for (auto& Input : MappedAbilities)
		{
			AbilitiesInputComponent->SetupBinding(this, Input.Key);
		}
	}
}

void UProfessionComponent::TeardownInputBinding()
{
	ensure(AbilitiesInputComponent);
	bIsActiveAbilitiesInput = false;

	for (auto& Input : MappedAbilities)
	{
		AbilitiesInputComponent->TeardownBinding(Input.Key);
	}
}

void UProfessionComponent::ReportInput(UInputAction* InputAction, FGameplayTag Tag, float Magnitude)
{
	int32 AbilityID = QueryAbility(InputAction);

	FGameplayEventData Payload;
	Payload.EventTag = Tag;
	Payload.EventMagnitude = Magnitude;

	if (Tag == CombatFrameworkTags::Combat_Event_Input_Started)
	{
		RegisteredAbilitySystemComponent->PressInputID(AbilityID);
	}

	RegisteredAbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);

	if (Tag == CombatFrameworkTags::Combat_Event_Input_Canceled || Tag == CombatFrameworkTags::Combat_Event_Input_Completed)
	{
		RegisteredAbilitySystemComponent->ReleaseInputID(AbilityID);
	}
}
