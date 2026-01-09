// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Components/CombatStanceComponent.h"
#include "AbilitySystem/Components/CombatAbilitySystemComponent.h"
#include "Components/AbilitiesInputComponent.h"
#include "CombatFrameworkTags.h"
#include "CombatCore/CombatGameplayTags.h"
#include "AbilitySystemGlobals.h"

FName UCombatStanceComponent::DefaultStance(TEXT("Default"));

UCombatStanceComponent::UCombatStanceComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.SetTickFunctionEnable(true);

	ActiveAbilityID = INDEX_NONE;
}

void UCombatStanceComponent::InitializeComponent()
{
	RegisterStanceAbilities();

	Super::InitializeComponent();

	SwitchCombatStance(DefaultStance);
}

void UCombatStanceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (RegisteredAbilitySystemComponent)
	{
		if (StashInputAction && !RegisteredAbilitySystemComponent->HasMatchingGameplayTag(CombatGameplayTags::Combat_AbilitySystem_Status_AheadInput))
		{
			PopInput();
		}
	}
}

int UCombatStanceComponent::AddAbility(TSoftClassPtr<class UGameplayAbility> AbilityType, FName Name, UTexture* Icon, UInputAction* InputAction)
{
	return Super::AddAbility(AbilityType, Name, Icon, InputAction);
}

void UCombatStanceComponent::RemoveAbility(int32 AbilityID)
{
	Super::RemoveAbility(AbilityID);
}

int32 UCombatStanceComponent::QueryAbility(const UInputAction* InputAction) const
{
	const TMap<UInputAction*, int32>* StanceMappedAbilities = StanceAbilitiesMap.Find(Stance);
	if (StanceMappedAbilities)
	{
		const int32* AbilityID = StanceMappedAbilities->Find(InputAction);
		if (AbilityID && *AbilityID)
		{
			return *AbilityID;
		}
	}
	return Super::QueryAbility(InputAction);
}

FAbilityInfo UCombatStanceComponent::GetAbilityInfo(int32 AbilityID) const
{
	return Super::GetAbilityInfo(AbilityID);
}

void UCombatStanceComponent::SetupInputBinding()
{
	Super::SetupInputBinding();

	for (auto& StanceAbilities : StanceAbilitiesMap)
	{
		for (auto& AbilityMap : StanceAbilities.Value)
		{
			if (AbilityMap.Key)
			{
				AbilitiesInputComponent->SetupBinding(this, AbilityMap.Key);
			}
		}
	}
}

void UCombatStanceComponent::TeardownInputBinding()
{
	Super::TeardownInputBinding();

	for (auto& StanceAbilities : StanceAbilitiesMap)
	{
		for (auto& AbilityMap : StanceAbilities.Value)
		{
			if (AbilityMap.Key)
			{
				AbilitiesInputComponent->TeardownBinding(AbilityMap.Key);
			}
		}
	}
}

void UCombatStanceComponent::ReportInput(UInputAction* InputAction, FGameplayTag Tag, float Magnitude)
{
	int32 AbilityID = QueryAbility(InputAction);
	if (AbilityID <= 0 || !RegisteredAbilitySystemComponent)
	{
		return;
	}

	if (AbilityID != ActiveAbilityID)
	{
		FGameplayAbilitySpec InputAbilitySpec;
		FGameplayAbilitySpec ActiveAbilitySpec;
		for (FGameplayAbilitySpec& Spec : RegisteredAbilitySystemComponent->GetActivatableAbilities())
		{
			if (Spec.InputID == AbilityID)
			{
				InputAbilitySpec = Spec;
			}
			else if (Spec.InputID == ActiveAbilityID)
			{
				ActiveAbilitySpec = Spec;
			}
		}

		if (!InputAbilitySpec.Ability->AbilityTags.HasTagExact(CombatGameplayTags::Combat_AbilitySystem_AbilityType_Move))
		{
			// 仅招式类技能受体态控制。
		}
		else if (ActiveAbilitySpec.IsActive())
		{
			if (InputAbilitySpec.IsActive() || InputAbilitySpec.Ability->CanActivateAbility(InputAbilitySpec.Handle, RegisteredAbilitySystemComponent->AbilityActorInfo.Get()))
			{
				// 本输入的技能在激活中或能激活，说明与激活中技能是相对独立的，可以直接上报输入而不造成其他影响。
			}
			else if (RegisteredAbilitySystemComponent->HasMatchingGameplayTag(CombatGameplayTags::Combat_AbilitySystem_Status_AheadInput))
			{
				// 处于提前输入状态，暂存输入操作并退出。
				StashInput(InputAction, Tag, Magnitude);
				return;
			}
			else
			{
				// 如果当前技能仍在激活中，则以上次技能为准。
				AbilityID = ActiveAbilitySpec.InputID;
			}
		}
		else
		{
			if (InputAbilitySpec.IsActive())
			{

			}
			else if (Tag == CombatFrameworkTags::Combat_Event_Input_Started)
			{
				ActiveAbilityID = AbilityID;
			}
			else
			{
				return;
			}
		}
	}

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

void UCombatStanceComponent::StashInput(UInputAction* InputAction, FGameplayTag Tag, float Magnitude)
{
	if (!StashInputAction)
	{
		// 暂存的预输入必须从Started开始；但不一定以Completed结束（可以在激活前、激活中Triggered、Canceled、Completed）。
		if (Tag == CombatFrameworkTags::Combat_Event_Input_Started)
		{
			StashInputAction = InputAction;
		}
		else
		{
			return;
		}
	}
	else
	{
		if (Tag == CombatFrameworkTags::Combat_Event_Input_Started)
		{
			StashInputAction = InputAction;
			StashInputList.Empty();
		}
		else
		{
			// 忽略其他操作的非按下行为和连续的重复行为。
			if (InputAction != StashInputAction)
			{
				return;
			}
			else if (Tag == StashInputList.Last().Tag)
			{
				return;
			}
		}
	}

	FInputStashInfo InputInfo;
	InputInfo.Tag = Tag;
	InputInfo.Magnitude = Magnitude;
	StashInputList.Add(InputInfo);
}

void UCombatStanceComponent::PopInput()
{
	auto FirstInputInfo = StashInputList.CreateIterator();
	if (StashInputAction && FirstInputInfo)
	{
		ReportInput(StashInputAction, FirstInputInfo->Tag, FirstInputInfo->Magnitude);
		FirstInputInfo.RemoveCurrent();
	}

	if (StashInputList.IsEmpty())
	{
		StashInputAction = nullptr;
	}
}

void UCombatStanceComponent::RegisterStanceAbilities()
{
	UCombatAbilitySystemComponent* CombatAbilitySystemComponent = Cast<UCombatAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetOwner()));
	ensure(CombatAbilitySystemComponent);

	if (!StanceAbilitiesTable.IsNull())
	{
		UDataTable* Table = StanceAbilitiesTable.LoadSynchronous();

		// 将表中所有出现的技能授予GASC，用GASC返回的AbilityID替换原先的GA类，在本组件用二级Map存储各体态各操作对应的技能，延用父类存储技能的信息。
		TArray<FName> StanceNames = Table->GetRowNames();
		for (const FName StanceName : StanceNames)
		{
			FStanceAbilities* AbilityInfo = Table->FindRow<FStanceAbilities>(StanceName, TEXT("CombatStance"));
			if (AbilityInfo)
			{
				TMap<UInputAction*, int32> StanceMappedAbilities;

				auto ActionAbilityInfo = AbilityInfo->StanceAbilities.CreateIterator();
				while (ActionAbilityInfo)
				{
					TSoftClassPtr<class UGameplayAbility> AbilityType = ActionAbilityInfo.Value().AbilityType;
					if (!AbilityType.IsNull())
					{
						// 弃用InputAction以跳过输入绑定流程，便于Map和Binding统一处理。
						int32 GrintedAbilityID = AddAbility(AbilityType, ActionAbilityInfo.Value().Name, ActionAbilityInfo.Value().Icon);

						UInputAction* Action = ActionAbilityInfo.Key().LoadSynchronous();
						StanceMappedAbilities.Add(Action, GrintedAbilityID);
					}
					++ActionAbilityInfo;
				}
				StanceAbilitiesMap.Add(StanceName, StanceMappedAbilities);
			}
		}
	}

	if (CombatAbilitySystemComponent->CombatStanceComponent && CombatAbilitySystemComponent->CombatStanceComponent != this)
	{
		CombatAbilitySystemComponent->CombatStanceComponent->TeardownInputBinding();
	}
	CombatAbilitySystemComponent->CombatStanceComponent = this;
}

void UCombatStanceComponent::SwitchCombatStance(FName NewStance)
{
	Stance = NewStance;
	SwitchCombatStanceDelegate.Broadcast(NewStance);
}

void UCombatStanceComponent::SettleStanceAbility()
{
	ActiveAbilityID = INDEX_NONE;

	if (!StashInputAction)
	{
		SwitchCombatStance(DefaultStance);
	}
}
