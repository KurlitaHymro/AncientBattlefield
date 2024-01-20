// Fill out your copyright notice in the Description page of Project Settings.


#include "PropertyFragment/PropertyFragment_EntityLink.h"
#include "DataRegistrySubsystem.h"
#include "Item/ItemObject.h"
#include "Components/GameFrameworkComponentManager.h"

FGameplayTag UPropertyFragment_EntityLink::PropertyTag(FGameplayTag::RequestGameplayTag(TEXT("InventorySystem.Property.EntityLink")));
FName UPropertyFragment_EntityLink::RegistryType(TEXT("EntityLinkRegistry"));

void UPropertyFragment_EntityLink::InitFromRegistry(FName Template)
{
	auto Registry = UDataRegistrySubsystem::Get()->GetRegistryForType(GetRegistryTypeName());
	if (Registry)
	{
		auto Prefab = Registry->GetCachedItem<FPropertyFragmentEntityLink>(FDataRegistryId(GetRegistryTypeName(), Template));
		PropertyFragment = *Prefab;
	}
}

FGameplayTag UPropertyFragment_EntityLink::GetPropertyTag()
{
	return PropertyTag;
}

FName UPropertyFragment_EntityLink::GetRegistryTypeName()
{
	return RegistryType;
}

void UPropertyFragment_EntityLink::SpawnEntity()
{
	if (PropertyFragment.EntityType != nullptr && Entity == nullptr)
	{
		FActorSpawnParameters SpawnConfig;
		SpawnConfig.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		TSubclassOf<AActor> EntityType = PropertyFragment.EntityType.LoadSynchronous();
		if (EntityType)
		{
			Entity = Cast<AEntityActor>(GetWorld()->SpawnActor(EntityType));
			Entity->ItemObject = Owner;
			ItemSpawnEntityDelegate.Broadcast(Entity);
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

// Sets default values
AEntityActor::AEntityActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}