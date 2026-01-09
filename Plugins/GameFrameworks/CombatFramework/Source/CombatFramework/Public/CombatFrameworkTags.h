// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

/**
 * 
 */
namespace CombatFrameworkTags
{
	COMBATFRAMEWORK_API FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	COMBATFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Combat_Event_Input_Triggered);
	COMBATFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Combat_Event_Input_Started);
	COMBATFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Combat_Event_Input_Ongoing);
	COMBATFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Combat_Event_Input_Canceled);
	COMBATFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Combat_Event_Input_Completed);
};