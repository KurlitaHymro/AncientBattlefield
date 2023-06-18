// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSystem/PropertyFragment/PropertyFragment_Equipment.h"

void UPropertyFragment_Equipment::SpawnEntity()
{
	if (EntityType)
	{
		Entity = GetWorld()->SpawnActor(EntityType);
		EquipmentMesh = Entity->GetComponentByClass<UMeshComponent>();
		if (EquipmentMesh == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Equipment Entity Actor No Mesh"));
		}
	}
}

void UPropertyFragment_Equipment::DestroyEntity()
{
	if (Entity)
	{
		Entity->Destroy();
		Entity = nullptr;
	}
}

void UPropertyFragment_Equipment::PutOn()
{
	if (ParentMesh != nullptr && EntityType != nullptr && AttachSocket.IsValid())
	{
		FAttachmentTransformRules Rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
		if (Entity == nullptr)
		{
			SpawnEntity();
		}
		Entity->AttachToComponent(ParentMesh, Rules, AttachSocket);
	}
}

void UPropertyFragment_Equipment::TakeOff()
{
}

UMeshComponent* UPropertyFragment_Equipment::GetMesh()
{
	return EquipmentMesh;
}
