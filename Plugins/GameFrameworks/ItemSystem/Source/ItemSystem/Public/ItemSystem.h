// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "NativeGameplayTags.h"

DECLARE_LOG_CATEGORY_EXTERN(LogItemSystem, Log, All)

class FItemSystemModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

namespace ItemSystemGameplayTags
{
	ITEMSYSTEM_API struct FGameplayTag FindTagByString(const FString& TagString, bool bMatchPartialString = false);

	ITEMSYSTEM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ItemSystem_Property_Prop);
	ITEMSYSTEM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ItemSystem_Property_Inventory);
	ITEMSYSTEM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ItemSystem_Property_EntityLink);
	ITEMSYSTEM_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(ItemSystem_Property_Equipment);
};