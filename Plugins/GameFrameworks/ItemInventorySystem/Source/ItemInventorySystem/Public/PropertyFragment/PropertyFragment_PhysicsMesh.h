// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemPropertyFragment.h"
#include "PropertyFragment_PhysicsMesh.generated.h"

UENUM(BlueprintType)
enum class EEntityState : uint8
{
	OnlyMesh,
	PhysicsMesh,
	CollisionMesh,
	PhysicsCollisionMesh
};

/**
 * 
 */
UCLASS()
class ITEMINVENTORYSYSTEM_API UPropertyFragment_PhysicsMesh : public UItemPropertyFragment
{
	GENERATED_BODY()

public:
	virtual FGameplayTag GetPropertyTag() override;

	virtual FGameplayTagContainer GetRequiredTags() override;

public:
	UFUNCTION(BlueprintCallable)
	void SetEntityState(UMeshComponent* Mesh, EEntityState State);

	UFUNCTION(BlueprintCallable)
	void Abandon(float LocationOffset = 150.f, FRotator Rotator = FRotator::ZeroRotator);

	UFUNCTION(BlueprintCallable)
	void Pickup(UInventoryComponent* TargetInventory);

public:
	static FGameplayTag PropertyTag;

	static FName RegistryType;
};
