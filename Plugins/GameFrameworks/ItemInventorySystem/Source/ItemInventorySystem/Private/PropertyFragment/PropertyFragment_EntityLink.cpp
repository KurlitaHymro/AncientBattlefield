// Fill out your copyright notice in the Description page of Project Settings.


#include "PropertyFragment/PropertyFragment_EntityLink.h"
#include "DataRegistrySubsystem.h"

void UPropertyFragment_EntityLink::InitFromDataTable(const UDataTable* DataTable, FName PrefabName)
{
	FPropertyFragmentEntityLink* Prefab = DataTable->FindRow<FPropertyFragmentEntityLink>(PrefabName, DataTable->GetName(), true);
	if (Prefab)
	{
		PropertyFragment = *Prefab;
	}
}

void UPropertyFragment_EntityLink::InitFromRegistry(const FName RegistryType, FName PrefabName)
{
	auto Registry = UDataRegistrySubsystem::Get()->GetRegistryForType(RegistryType);
	if (Registry)
	{
		auto Prefab = Registry->GetCachedItem<FPropertyFragmentEntityLink>(FDataRegistryId(RegistryType, PrefabName));
		PropertyFragment = *Prefab;
	}
}

FName UPropertyFragment_EntityLink::GetRegistryTypeName()
{
	return FName("EntityLinkRegistry");
}

void UPropertyFragment_EntityLink::SpawnEntity()
{
	if (PropertyFragment.EntityType != nullptr)
	{
		FActorSpawnParameters SpawnConfig;
		SpawnConfig.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		auto World = GetWorld();
		TSubclassOf<AActor> EntityType = PropertyFragment.EntityType.LoadSynchronous();
		if (EntityType)
		{
			Entity = World->SpawnActor(EntityType);
		}
	}
}

void UPropertyFragment_EntityLink::DestroyEntity()
{
	if (Entity)
	{
		Entity->Destroy();
		Entity = nullptr;
	}
}