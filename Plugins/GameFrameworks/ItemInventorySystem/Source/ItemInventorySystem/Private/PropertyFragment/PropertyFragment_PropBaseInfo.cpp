// Fill out your copyright notice in the Description page of Project Settings.


#include "PropertyFragment/PropertyFragment_PropBaseInfo.h"
#include "DataRegistrySubsystem.h"

void UPropertyFragment_PropBaseInfo::InitFromDataTable(const UDataTable* DataTable, FName PrefabName)
{
	FPropertyFragmentPropBaseInfo* Prefab = DataTable->FindRow<FPropertyFragmentPropBaseInfo>(PrefabName, DataTable->GetName(), true);
	if (Prefab)
	{
		PropertyFragment = *Prefab;
	}
}

void UPropertyFragment_PropBaseInfo::InitFromRegistry(const FName RegistryType, FName PrefabName)
{
	auto Registry = UDataRegistrySubsystem::Get()->GetRegistryForType(RegistryType);
	if (Registry)
	{
		auto Prefab = Registry->GetCachedItem<FPropertyFragmentPropBaseInfo>(FDataRegistryId(RegistryType, PrefabName));
		PropertyFragment = *Prefab;
	}
}