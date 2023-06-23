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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> EntityType;

public:
	UFUNCTION(BlueprintCallable)
	void SpawnEntity();

	UFUNCTION(BlueprintCallable)
	void DestroyEntity();

	UFUNCTION(BlueprintCallable)
	AActor* GetEntity();

private:
	AActor* Entity;
};
