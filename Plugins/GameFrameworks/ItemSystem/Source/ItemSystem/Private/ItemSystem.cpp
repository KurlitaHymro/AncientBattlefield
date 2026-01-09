// Copyright Epic Games, Inc. All Rights Reserved.

#include "ItemSystem.h"
#include "GameplayTagsManager.h"

#define LOCTEXT_NAMESPACE "FItemSystemModule"

void FItemSystemModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FItemSystemModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FItemSystemModule, ItemSystem)

DEFINE_LOG_CATEGORY(LogItemSystem)

namespace ItemSystemGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(ItemSystem_Property_Prop, "ItemSystem.Property.Prop");
	UE_DEFINE_GAMEPLAY_TAG(ItemSystem_Property_Inventory, "ItemSystem.Property.Inventory");
	UE_DEFINE_GAMEPLAY_TAG(ItemSystem_Property_EntityLink, "ItemSystem.Property.EntityLink");
	UE_DEFINE_GAMEPLAY_TAG(ItemSystem_Property_Equipment, "ItemSystem.Property.Equipment");

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
					UE_LOG(LogItemSystem, Display, TEXT("Could not find exact match for tag [%s] but found partial match on tag [%s]."), *TagString, *TestTag.ToString());
					Tag = TestTag;
					break;
				}
			}
		}
		return Tag;
	}
}
