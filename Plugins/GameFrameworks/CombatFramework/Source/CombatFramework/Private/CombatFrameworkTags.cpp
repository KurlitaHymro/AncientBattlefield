// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatFrameworkTags.h"
#include "Engine/EngineTypes.h"
#include "GameplayTagsManager.h"

namespace CombatFrameworkTags
{
	UE_DEFINE_GAMEPLAY_TAG(Combat_Event_Input_Triggered, "CombatFramework.Event.Input.Triggered");
	UE_DEFINE_GAMEPLAY_TAG(Combat_Event_Input_Started, "CombatFramework.Event.Input.Started");
	UE_DEFINE_GAMEPLAY_TAG(Combat_Event_Input_Ongoing, "CombatFramework.Event.Input.Ongoing");
	UE_DEFINE_GAMEPLAY_TAG(Combat_Event_Input_Canceled, "CombatFramework.Event.Input.Canceled");
	UE_DEFINE_GAMEPLAY_TAG(Combat_Event_Input_Completed, "CombatFramework.Event.Input.Completed");

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