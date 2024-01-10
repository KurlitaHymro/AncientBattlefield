// Fill out your copyright notice in the Description page of Project Settings.


#include "PropertyFragment/PropertyFragment_PhysicsMesh.h"
#include "PropertyFragment/PropertyFragment_EntityLink.h"
#include "Item/ItemObject.h"
#include "InventoryComponent.h"
#include "Kismet/GameplayStatics.h"

FName UPropertyFragment_PhysicsMesh::GetPropertyTagName()
{
	return FName("InventorySystem.Property.PhysicsMesh");
}

void UPropertyFragment_PhysicsMesh::SetEntityState(UMeshComponent* Mesh, EEntityState State)
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

void UPropertyFragment_PhysicsMesh::Abandon(float LocationOffset, FRotator Rotator)
{
	UPropertyFragment_EntityLink* EntityLink = Owner->FindPropertyFragment<UPropertyFragment_EntityLink>();
	{
		if (EntityLink && Owner->BelongingInventory)
		{
			AActor* BelongingActor = Owner->BelongingInventory->GetOwner();
			Owner->BelongingInventory->RemoveItem(Owner);
			if (EntityLink->GetEntity() == nullptr)
			{
				EntityLink->SpawnEntity();
				UMeshComponent* Mesh = EntityLink->GetEntity()->FindComponentByClass<UMeshComponent>();
				FVector Location = BelongingActor->GetActorLocation() + BelongingActor->GetActorForwardVector() * LocationOffset;
				EntityLink->GetEntity()->SetActorLocation(Location);
				EntityLink->GetEntity()->SetActorRotation(Rotator);
				SetEntityState(Mesh, EEntityState::PhysicsCollisionMesh);
			}
		}
	}
}

void UPropertyFragment_PhysicsMesh::Pickup(UInventoryComponent* TargetInventory)
{
	UPropertyFragment_EntityLink* EntityLink = Owner->FindPropertyFragment<UPropertyFragment_EntityLink>();
	{
		if (EntityLink && EntityLink->GetEntity() != nullptr)
		{
			EntityLink->DestroyEntity();
			TargetInventory->AddItem(Owner, TargetInventory->FindVacancy(Owner));
		}
	}
}
