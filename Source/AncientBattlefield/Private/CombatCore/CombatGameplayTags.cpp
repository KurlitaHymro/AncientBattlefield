// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatCore/CombatGameplayTags.h"
#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"

namespace CombatGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Combat_AbilitySystem_Event_SwitchStance, "AbilitySystem.Event.SwitchStance");
	UE_DEFINE_GAMEPLAY_TAG(Combat_AbilitySystem_Status_AheadInput, "AbilitySystem.Status.AheadInput");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Combat_AbilitySystem_AbilityType_Behavior, "AbilitySystem.AbilityType.Behavior", "行为");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Combat_AbilitySystem_AbilityType_Move, "AbilitySystem.AbilityType.Move", "招式");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Combat_AbilitySystem_AbilityType_Theurgys, "AbilitySystem.AbilityType.Theurgys", "仙法");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Combat_AbilitySystem_AbilityType_Stratagem, "AbilitySystem.AbilityType.Stratagem", "谋略");

	UE_DEFINE_GAMEPLAY_TAG(Combat_AbilitySystem_Event_HitTraceBegin, "AbilitySystem.Event.HitTraceBegin");
	UE_DEFINE_GAMEPLAY_TAG(Combat_AbilitySystem_Event_HitTraceEnd, "AbilitySystem.Event.HitTraceEnd");
	UE_DEFINE_GAMEPLAY_TAG(Combat_AbilitySystem_Event_MeleeWeapon, "AbilitySystem.Event.MeleeWeapon");

	UE_DEFINE_GAMEPLAY_TAG(Combat_AbilitySystem_Status_Running, "AbilitySystem.Status.Running");
	UE_DEFINE_GAMEPLAY_TAG(Combat_AbilitySystem_Status_Falling, "AbilitySystem.Status.Falling");
	UE_DEFINE_GAMEPLAY_TAG(Combat_AbilitySystem_Status_Defensive, "AbilitySystem.Status.Defensive");

	UE_DEFINE_GAMEPLAY_TAG(Combat_ItemSystem_Property_Equipment_Helmet, "ItemSystem.Property.Equipment.Helmet");
	UE_DEFINE_GAMEPLAY_TAG(Combat_ItemSystem_Property_Equipment_Armor, "ItemSystem.Property.Equipment.Armor");
	UE_DEFINE_GAMEPLAY_TAG(Combat_ItemSystem_Property_Equipment_Belt, "ItemSystem.Property.Equipment.Belt");
	UE_DEFINE_GAMEPLAY_TAG(Combat_ItemSystem_Property_Equipment_Pants, "ItemSystem.Property.Equipment.Pants");
	UE_DEFINE_GAMEPLAY_TAG(Combat_ItemSystem_Property_Equipment_Boots, "ItemSystem.Property.Equipment.Boots");
	UE_DEFINE_GAMEPLAY_TAG(Combat_ItemSystem_Property_Equipment_Cape, "ItemSystem.Property.Equipment.Cape");
	UE_DEFINE_GAMEPLAY_TAG(Combat_ItemSystem_Property_Equipment_Weapon, "ItemSystem.Property.Equipment.Weapon");

	FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString)
	{
		const UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
		FGameplayTag Tag = Manager.RequestGameplayTag(FName(*TagString), false);

		if (!Tag.IsValid() && bMatchPartialString)
		{
			FGameplayTagContainer AllTags;
			Manager.RequestAllGameplayTags(AllTags, true);

			for (const FGameplayTag& TestTag : AllTags)
			{
				if (TestTag.ToString().Contains(TagString))
				{
					UE_LOG(LogTemp, Display, TEXT("Could not find exact match for tag [%s] but found partial match on tag [%s]."), *TagString, *TestTag.ToString());
					Tag = TestTag;
					break;
				}
			}
		}
		return Tag;
	}
}