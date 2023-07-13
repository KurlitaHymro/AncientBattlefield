// Fill out your copyright notice in the Description page of Project Settings.


#include "PropertyFragment/PropertyFragment_EntityLink.h"

void UPropertyFragment_EntityLink::SpawnEntity()
{
	if (EntityType)
	{
		FActorSpawnParameters SpawnConfig;
		SpawnConfig.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		auto World = GetWorld();
		Entity = World->SpawnActor(EntityType);
		Mesh = Entity->GetComponentByClass<UMeshComponent>();
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