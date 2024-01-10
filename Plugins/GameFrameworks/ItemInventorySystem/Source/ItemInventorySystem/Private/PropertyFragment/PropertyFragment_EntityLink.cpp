// Fill out your copyright notice in the Description page of Project Settings.


#include "PropertyFragment/PropertyFragment_EntityLink.h"
#include "DataRegistrySubsystem.h"
#include "Components/GameFrameworkComponentManager.h"

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

FName UPropertyFragment_EntityLink::GetPropertyTagName()
{
	return FName("InventorySystem.Property.EntityLink");
}

FName UPropertyFragment_EntityLink::GetRegistryTypeName()
{
	return FName("EntityLinkRegistry");
}

void UPropertyFragment_EntityLink::SpawnEntity()
{
	if (PropertyFragment.EntityType != nullptr && Entity == nullptr)
	{
		FActorSpawnParameters SpawnConfig;
		SpawnConfig.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		auto World = GetWorld();
		TSubclassOf<AActor> EntityType = PropertyFragment.EntityType.LoadSynchronous();
		if (EntityType)
		{
			Entity = Cast<AEntityActor>(World->SpawnActor(EntityType));
			Entity->ItemObject = Owner;
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

// Called when the game starts or when spawned
void AEntityActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEntityActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}