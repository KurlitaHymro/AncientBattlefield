// Fill out your copyright notice in the Description page of Project Settings.


#include "PropertyFragment/PropertyFragment_PropBaseInfo.h"
#include "DataRegistrySubsystem.h"

FGameplayTag UPropertyFragment_PropBaseInfo::PropertyTag(FGameplayTag::RequestGameplayTag(TEXT("InventorySystem.Property.PropBaseInfo")));
FName UPropertyFragment_PropBaseInfo::RegistryType(TEXT("PropBaseInfoRegistry"));

void UPropertyFragment_PropBaseInfo::InitFromRegistry(FName Template)
{
	auto Registry = UDataRegistrySubsystem::Get()->GetRegistryForType(GetRegistryTypeName());
	if (Registry)
	{
		auto Prefab = Registry->GetCachedItem<FPropertyFragmentPropBaseInfo>(FDataRegistryId(GetRegistryTypeName(), Template));
		PropertyFragment = *Prefab;
	}
}

FGameplayTag UPropertyFragment_PropBaseInfo::GetPropertyTag()
{
	return PropertyTag;
}

FName UPropertyFragment_PropBaseInfo::GetRegistryTypeName()
{
	return RegistryType;
}