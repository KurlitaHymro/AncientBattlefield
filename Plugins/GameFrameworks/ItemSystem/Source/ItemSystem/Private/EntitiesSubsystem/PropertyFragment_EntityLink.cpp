// Fill out your copyright notice in the Description page of Project Settings.


#include "EntitiesSubsystem/PropertyFragment_EntityLink.h"
#include "InventorySubsystem/PropertyFragment_Inventory.h"
#include "InventorySubsystem/Components/InventoryComponent.h"
#include "DataRegistrySubsystem.h"
#include "Item/ItemObject.h"
#include "ItemSystem.h"

#define LOCTEXT_NAMESPACE "PropertyFragment_EntityLink"

FName UPropertyFragment_EntityLink::RegistryType(TEXT("EntityLinkRegistry"));

void UPropertyFragment_EntityLink::Setup(const FName Template)
{
	Super::Setup(Template);

	if (UPropertyFragment_Inventory * Inventory = OwnerItem->FindPropertyFragment<UPropertyFragment_Inventory>())
	{
		Inventory->RegisterInventoryOperation(7, this, FName("Drop"), LOCTEXT("PropertyOperation_Drop", "Drop"));
	}
}

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
	return ItemSystemGameplayTags::ItemSystem_Property_EntityLink;
}

FName UPropertyFragment_EntityLink::GetRegistryTypeName()
{
	return RegistryType;
}

void UPropertyFragment_EntityLink::SpawnEntity(FVector const Location, FRotator const Rotation)
{
	if (PropertyFragment.EntityType != nullptr && Entity == nullptr)
	{
		FActorSpawnParameters SpawnConfig;
		SpawnConfig.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		TSubclassOf<AActor> EntityType = PropertyFragment.EntityType.LoadSynchronous();
		if (EntityType)
		{
			Entity = GetWorld()->SpawnActor(EntityType, &Location, &Rotation, SpawnConfig);
			EntityLinkSpawnEntityDelegate.Broadcast(Entity);
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

void UPropertyFragment_EntityLink::Drop()
{
	if (UPropertyFragment_Inventory* Inventory = OwnerItem->FindPropertyFragment<UPropertyFragment_Inventory>())
	{
		UInventoryComponent* InventoryComponent = Inventory->Inventory;
		ensure(InventoryComponent);

		InventoryComponent->Remove(OwnerItem);
		if (Entity)
		{
			Entity->DetachRootComponentFromParent(false);
		}
		else
		{
			AActor* InventoryOwner = InventoryComponent->GetOwner();
			FVector Location = InventoryOwner->GetActorLocation() + InventoryOwner->GetActorForwardVector() * 50.f;
			SpawnEntity(Location);
		}

		if (UMeshComponent* Mesh = Entity->GetComponentByClass<UMeshComponent>())
		{
			Mesh->SetSimulatePhysics(true);
			Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		}
	}
}

#undef LOCTEXT_NAMESPACE