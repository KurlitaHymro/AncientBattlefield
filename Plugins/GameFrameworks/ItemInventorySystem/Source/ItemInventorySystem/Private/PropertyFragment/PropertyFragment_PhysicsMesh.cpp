// Fill out your copyright notice in the Description page of Project Settings.


#include "PropertyFragment/PropertyFragment_PhysicsMesh.h"
#include "PropertyFragment/PropertyFragment_EntityLink.h"
#include "Item/ItemObject.h"
#include "InventoryComponent.h"

//void UPropertyFragment_PhysicsMesh::Instantiate(UItemObject* Owner)
//{
//	Super::Instantiate(Owner);
//
//	UPropertyFragment_EntityLink* EntityLink = GetOwner()->FindPropertyFragment<UPropertyFragment_EntityLink>();
//	if (EntityLink && Mesh == nullptr)
//	{
//		Mesh = Mesh != nullptr ? Mesh : EntityLink->GetEntity()->FindComponentByClass<UMeshComponent>();
//		Mesh = Mesh != nullptr ? Mesh : EntityLink->GetEntity()->FindComponentByClass<UStaticMeshComponent>();
//		Mesh = Mesh != nullptr ? Mesh : EntityLink->GetEntity()->FindComponentByClass<USkeletalMeshComponent>();
//	}
//
//	if (Owner->BelongingInventory == nullptr)
//	{
//		SetEntityState(EEntityState::PhysicsCollisionMesh);
//	}
//}

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
	UPropertyFragment_EntityLink* EntityLink = Owner->FindPropertyFragment<UPropertyFragment_EntityLink>();
	{
		if (EntityLink->GetEntity() == nullptr)
		{
			EntityLink->SpawnEntity();

			AActor* BelongingActor = Owner->BelongingInventory->GetOwner();
			FVector Location = BelongingActor->GetActorLocation() + BelongingActor->GetActorForwardVector() * 100.f;
			EntityLink->GetEntity()->SetActorLocation(Location);
		}
		Owner->BelongingInventory->RemoveItem(Owner);
	}
	SetEntityState(EEntityState::PhysicsCollisionMesh);
}

void UPropertyFragment_PhysicsMesh::Pickup(UInventoryComponent* TargetInventory)
{
	if (Owner == nullptr || TargetInventory == nullptr)
	{
		return;
	}
	UPropertyFragment_EntityLink* EntityLink = Owner->FindPropertyFragment<UPropertyFragment_EntityLink>();
	{
		if (EntityLink && EntityLink->GetEntity() != nullptr)
		{
			SetEntityState(EEntityState::OnlyMesh);
			EntityLink->DestroyEntity();
		}
		TargetInventory->AddItem(Owner, TargetInventory->FindVacancy());
	}
}
