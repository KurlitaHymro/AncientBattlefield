// Fill out your copyright notice in the Description page of Project Settings.


#include "PropertyFragment/PropertyFragment_SimulatePhysics.h"
#include "PropertyFragment/PropertyFragment_EntityLink.h"
#include "Item/ItemObject.h"
#include "InventoryComponent.h"

void UPropertyFragment_SimulatePhysics::EnableSimulatePhysics()
{
	auto Item = GetOwner();
	UPropertyFragment_EntityLink* EntityLink = Item->FindPropertyFragment<UPropertyFragment_EntityLink>();
	if (EntityLink && EntityLink->GetMesh() != nullptr)
	{
		Mesh = EntityLink->GetMesh();
		Mesh->SetSimulatePhysics(true);
		Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}
}

void UPropertyFragment_SimulatePhysics::DisableSimulatePhysics()
{
	if (Mesh)
	{
		Mesh->SetSimulatePhysics(false);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UPropertyFragment_SimulatePhysics::Abandon()
{
	auto Item = GetOwner();
	UPropertyFragment_EntityLink* EntityLink = Item->FindPropertyFragment<UPropertyFragment_EntityLink>();
	{
		if (EntityLink->GetEntity() == nullptr)
		{
			EntityLink->SpawnEntity();

			AActor* Owner = Item->BelongingInventory->GetOwner();
			FVector Location = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 50.f;
			EntityLink->GetEntity()->SetActorLocation(Location);
		}
		Item->BelongingInventory->RemoveItem(Item);
	}
	EnableSimulatePhysics();
}
