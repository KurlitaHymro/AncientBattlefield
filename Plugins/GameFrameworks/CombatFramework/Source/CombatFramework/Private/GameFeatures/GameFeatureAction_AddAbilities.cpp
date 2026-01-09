// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/GameFeatureAction_AddAbilities.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Components/RegisteredAbilitySystemComponent.h"
#include "Components/ProfessionComponent.h"
#include "Engine/AssetManager.h"
#include "GameFeaturesSubsystemSettings.h"
#include "InputAction.h"
#include "CombatFramework.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#define LOCTEXT_NAMESPACE "CombatGameFeatures"

void UGameFeatureAction_AddAbilities::OnGameFeatureActivating()
{
	if (!ensureAlways(ActiveExtensions.IsEmpty()) ||
		!ensureAlways(ComponentRequests.IsEmpty()))
	{
		Reset();
	}
	Super::OnGameFeatureActivating();
}

void UGameFeatureAction_AddAbilities::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);
	Reset();
}

#if WITH_EDITORONLY_DATA
void UGameFeatureAction_AddAbilities::AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData)
{
	if (UAssetManager::IsInitialized())
	{
		auto AddBundleAsset = [&AssetBundleData](const FTopLevelAssetPath& Path)
		{
			AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateClient, Path);
			AssetBundleData.AddBundleAsset(UGameFeaturesSubsystemSettings::LoadStateServer, Path);
		};

		for (const auto& Entry : EntriesList)
		{
			for (const auto& Ability : Entry.GrantedAbilities)
			{
				AddBundleAsset(Ability.AbilityType.ToSoftObjectPath().GetAssetPath());
				if (!Ability.InputAction.IsNull())
				{
					AddBundleAsset(Ability.InputAction.ToSoftObjectPath().GetAssetPath());
				}
			}

			for (const auto& Attributes : Entry.GrantedAttributes)
			{
				AddBundleAsset(Attributes.AttributeSetType.ToSoftObjectPath().GetAssetPath());
				if (!Attributes.InitializationData.IsNull())
				{
					AddBundleAsset(Attributes.InitializationData.ToSoftObjectPath().GetAssetPath());
				}
			}
		}
	}
}
#endif

#if WITH_EDITOR
EDataValidationResult UGameFeatureAction_AddAbilities::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = CombineDataValidationResults(Super::IsDataValid(Context), EDataValidationResult::Valid);

	int32 EntryIndex = 0;
	for (const auto& Entry : EntriesList)
	{
		if (Entry.ActorClass.IsNull())
		{
			Result = EDataValidationResult::Invalid;
			Context.AddError(FText::Format(LOCTEXT("EntryHasNullActor", "Null ActorClass at index {0} in AbilitiesList"), FText::AsNumber(EntryIndex)));
		}

		if (Entry.GrantedAbilities.IsEmpty() && Entry.GrantedAttributes.IsEmpty())
		{
			Result = EDataValidationResult::Invalid;
			Context.AddError(FText::Format(LOCTEXT("EntryHasNoAddOns", "Empty GrantedAbilities and GrantedAttributes at index {0} in AbilitiesList"), FText::AsNumber(EntryIndex)));
		}

		int32 AbilityIndex = 0;
		for (const auto& Ability : Entry.GrantedAbilities)
		{
			if (Ability.AbilityType.IsNull())
			{
				Result = EDataValidationResult::Invalid;
				Context.AddError(FText::Format(LOCTEXT("EntryHasNullAbility", "Null AbilityType at index {0} in AbilitiesList[{1}].GrantedAbilities"), FText::AsNumber(AbilityIndex), FText::AsNumber(EntryIndex)));
			}
			++AbilityIndex;
		}

		int32 AttributesIndex = 0;
		for (const auto& Attributes : Entry.GrantedAttributes)
		{
			if (Attributes.AttributeSetType.IsNull())
			{
				Result = EDataValidationResult::Invalid;
				Context.AddError(FText::Format(LOCTEXT("EntryHasNullAttributeSet", "Null AttributeSetType at index {0} in AbilitiesList[{1}].GrantedAttributes"), FText::AsNumber(AttributesIndex), FText::AsNumber(EntryIndex)));
			}
			++AttributesIndex;
		}

		++EntryIndex;
	}

	return Result;
}
#endif

void UGameFeatureAction_AddAbilities::AddToWorld(const FWorldContext& WorldContext)
{
	UWorld* World = WorldContext.World();
	UGameInstance* GameInstance = WorldContext.OwningGameInstance;

	if ((GameInstance != nullptr) && (World != nullptr) && World->IsGameWorld())
	{
		if (UGameFrameworkComponentManager* ComponentMan = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
		{
			int32 EntryIndex = 0;
			for (const auto& Entry : EntriesList)
			{
				if (!Entry.ActorClass.IsNull())
				{
					UGameFrameworkComponentManager::FExtensionHandlerDelegate AddAbilitiesDelegate = UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(
						this, &ThisClass::HandleActorExtension, EntryIndex);
					TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle = ComponentMan->AddExtensionHandler(Entry.ActorClass, AddAbilitiesDelegate);

					ComponentRequests.Add(ExtensionRequestHandle);
					EntryIndex++;
				}
			}
		}
	}
}

void UGameFeatureAction_AddAbilities::Reset()
{
	while (!ActiveExtensions.IsEmpty())
	{
		auto ExtensionIt = ActiveExtensions.CreateIterator();
		RemoveActorAbilities(ExtensionIt->Key);
	}

	ComponentRequests.Empty();
}

void UGameFeatureAction_AddAbilities::HandleActorExtension(AActor* Actor, FName EventName, int32 EntryIndex)
{
	if (EntriesList.IsValidIndex(EntryIndex))
	{
		const auto& Entry = EntriesList[EntryIndex];
		if (EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved/* || EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved*/)
		{
			RemoveActorAbilities(Actor);
		}
		else if (/*EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded || */EventName == UGameFrameworkComponentManager::NAME_GameActorReady)
		{
			AddActorAbilities(Actor, Entry);
		}
	}
}

void UGameFeatureAction_AddAbilities::AddActorAbilities(AActor* Actor, const FCombatAbilitiesEntry& Entry)
{
	if (URegisteredAbilitySystemComponent* AbilitySystemComponent = FindOrAddComponentForActor<URegisteredAbilitySystemComponent>(Actor, Entry.ActorClass))
	{
		UProfessionComponent* ProfessionComponent = FindOrAddComponentForActor<UProfessionComponent>(Actor, Entry.ActorClass);
		
		FActorExtensions AddedExtensions;
		AddedExtensions.AbilitySystemComponent = AbilitySystemComponent;
		AddedExtensions.ProfessionComponent = ProfessionComponent;
		AddedExtensions.Abilities.Reserve(Entry.GrantedAbilities.Num());
		AddedExtensions.Attributes.Reserve(Entry.GrantedAttributes.Num());

		// 以此方式赋予的能力应当是与流派或体态无关的角色固有能力（如跑动、跳跃、交互等），也具备独特的IA。不应与战斗相关，也不是赋予技能的唯一途径。
		for (const auto& Ability : Entry.GrantedAbilities)
		{
			if (!Ability.AbilityType.IsNull() && ProfessionComponent)
			{
				UInputAction* Action = Ability.InputAction.IsNull() ? nullptr : Ability.InputAction.LoadSynchronous();
				int32 AbilityID = ProfessionComponent->AddAbility(Ability.AbilityType, Ability.Name, Ability.Icon, Action);

				AddedExtensions.Abilities.Add(AbilityID);
			}
			else
			{
				UE_LOG(LogCombatFramework, Error, TEXT("Failed to find/add a profession component to '%s' -- are you sure it's a pawn class?"), *Actor->GetPathName());
			}
		}

		// 以此方式赋予的属性集应当是与流派无关的角色固有属性（如生命、移动速度等）。
		for (const auto& Attributes : Entry.GrantedAttributes)
		{
			if (!Attributes.AttributeSetType.IsNull())
			{
				TSubclassOf<UAttributeSet> SetType = Attributes.AttributeSetType.LoadSynchronous();
				if (SetType)
				{
					UAttributeSet* AttributeSetInstance = NewObject<UAttributeSet>(AbilitySystemComponent, SetType);
					if (!Attributes.InitializationData.IsNull())
					{
						UDataTable* InitData = Attributes.InitializationData.LoadSynchronous();
						if (InitData)
						{
							AttributeSetInstance->InitFromMetaDataTable(InitData);
						}
					}
					AddedExtensions.Attributes.Add(AttributeSetInstance);
					AbilitySystemComponent->AddAttributeSetSubobject(AttributeSetInstance);
				}
			}
		}
		ActiveExtensions.Add(Actor, AddedExtensions);
	}
	else
	{
		UE_LOG(LogCombatFramework, Error, TEXT("Failed to find/add an ability component to '%s'. Abilities will not be granted."), *Actor->GetPathName());
	}
}

void UGameFeatureAction_AddAbilities::RemoveActorAbilities(AActor* Actor)
{
	if (FActorExtensions* ActorExtensions = ActiveExtensions.Find(Actor))
	{
		if (ActorExtensions->AbilitySystemComponent && ActorExtensions->ProfessionComponent)
		{
			for (UAttributeSet* AttribSetInstance : ActorExtensions->Attributes)
			{
				ActorExtensions->AbilitySystemComponent->RemoveSpawnedAttribute(AttribSetInstance);
			}

			for (auto AbilityID : ActorExtensions->Abilities)
			{
				ActorExtensions->ProfessionComponent->RemoveAbility(AbilityID);
			}
		}
		ActiveExtensions.Remove(Actor);
	}
}

#undef LOCTEXT_NAMESPACE