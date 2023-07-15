// Fill out your copyright notice in the Description page of Project Settings.


#include "PropertyFragment/PropertyFragment_PhysicsMesh.h"
#include "PropertyFragment/PropertyFragment_EntityLink.h"
#include "Item/ItemObject.h"
#include "InventoryComponent.h"

void UPropertyFragment_PhysicsMesh::Instantiate(UItemObject* Owner)
{
	Super::Instantiate(Owner);

	UPropertyFragment_EntityLink* EntityLink = GetOwner()->FindPropertyFragment<UPropertyFragment_EntityLink>();
	if (EntityLink)
	{
		Mesh = Mesh != nullptr ? Mesh : EntityLink->GetEntity()->FindComponentByClass<UMeshComponent>();
		Mesh = Mesh != nullptr ? Mesh : EntityLink->GetEntity()->FindComponentByClass<UStaticMeshComponent>();
		Mesh = Mesh != nullptr ? Mesh : EntityLink->GetEntity()->FindComponentByClass<USkeletalMeshComponent>();
	}
}

void UPropertyFragment_PhysicsMesh::SetEntityState(EEntityState State)
{
	if (Mesh != nullptr)
	{
		switch (State)
		{
		case EEntityState::OnlyMesh:
			Mesh->SetSimulatePhysics(false);
			Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
		case EEntityState::PhysicsMesh:
			Mesh->SetSimulatePhysics(true);
			Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			break;
		case EEntityState::CollisionMesh:
			Mesh->SetSimulatePhysics(false);
			Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
			break;
		case EEntityState::PhysicsCollisionMesh:
			Mesh->SetSimulatePhysics(true);
			Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
			break;
		}
	}
}

void UPropertyFragment_PhysicsMesh::Abandon()
{
	auto Item = GetOwner();
	UPropertyFragment_EntityLink* EntityLink = Item->FindPropertyFragment<UPropertyFragment_EntityLink>();
	{
		if (EntityLink->GetEntity() == nullptr)
		{
			EntityLink->SpawnEntity();

			AActor* Owner = Item->BelongingInventory->GetOwner();
			FVector Location = Owner->GetActorLocation() + Owner->GetActorForwardVector() * 100.f;
			EntityLink->GetEntity()->SetActorLocation(Location);
		}
		Item->BelongingInventory->RemoveItem(Item);
	}
	SetEntityState(EEntityState::PhysicsCollisionMesh);
}

void UPropertyFragment_PhysicsMesh::Pickup(UInventoryComponent* Owner)
{
	auto Item = GetOwner();
	if (Item == nullptr || Owner == nullptr)
	{
		return;
	}
	UPropertyFragment_EntityLink* EntityLink = Item->FindPropertyFragment<UPropertyFragment_EntityLink>();
	{
		if (EntityLink && EntityLink->GetEntity() != nullptr)
		{
			SetEntityState(EEntityState::OnlyMesh);
			EntityLink->DestroyEntity();
		}
		Owner->AddItem(Item, Owner->FindVacancy());
	}
}
