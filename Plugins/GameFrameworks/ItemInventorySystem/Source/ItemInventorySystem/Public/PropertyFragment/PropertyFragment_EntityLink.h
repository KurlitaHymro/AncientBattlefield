// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemPropertyFragment.h"
#include "PropertyFragment_EntityLink.generated.h"

/**
 * 
 */
UCLASS()
class ITEMINVENTORYSYSTEM_API UPropertyFragment_EntityLink : public UItemPropertyFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InstanceEditable = true, ExposeOnSpawn = true))
	TSubclassOf<AActor> EntityType;

public:
	UFUNCTION(BlueprintCallable)
	void SpawnEntity();

	UFUNCTION(BlueprintCallable)
	void DestroyEntity();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AActor* GetEntity() { return Entity; };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UMeshComponent* GetMesh() { return Mesh; };

private:
	AActor* Entity;

	UMeshComponent* Mesh;
};