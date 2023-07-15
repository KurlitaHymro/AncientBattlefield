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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InstanceEditable = true, ExposeOnSpawn = trueTooltip = "Preferably set on spawn, or Ensure the existence of EntityLink which has Mesh"))
	UMeshComponent* Mesh;

protected:
	virtual void Instantiate(class UItemObject* Owner) override;

public:
	UFUNCTION(BlueprintCallable)
	void SetEntityState(EEntityState State);

	UFUNCTION(BlueprintCallable)
	void Abandon();

	UFUNCTION(BlueprintCallable)
	void Pickup(UInventoryComponent* Owner);
};
