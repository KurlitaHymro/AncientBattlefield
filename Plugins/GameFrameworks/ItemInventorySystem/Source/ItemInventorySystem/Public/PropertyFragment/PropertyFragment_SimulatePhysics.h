// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemPropertyFragment.h"
#include "PropertyFragment_SimulatePhysics.generated.h"

/**
 * 
 */
UCLASS()
class ITEMINVENTORYSYSTEM_API UPropertyFragment_SimulatePhysics : public UItemPropertyFragment
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void EnableSimulatePhysics();

	UFUNCTION(BlueprintCallable)
	void DisableSimulatePhysics();

	UFUNCTION(BlueprintCallable)
	void Abandon();

private:
	UMeshComponent* Mesh;
};
