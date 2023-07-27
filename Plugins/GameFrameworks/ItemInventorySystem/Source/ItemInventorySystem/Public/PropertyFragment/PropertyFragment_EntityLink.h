// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemPropertyFragment.h"
#include "PropertyFragment_EntityLink.generated.h"

USTRUCT(BlueprintType, meta = (DisplayName = "EntityLink"))
struct FPropertyFragmentEntityLink : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftClassPtr<AActor> EntityType;
};

/**
 * 
 */
UCLASS()
class ITEMINVENTORYSYSTEM_API UPropertyFragment_EntityLink : public UItemPropertyFragment
{
	GENERATED_BODY()

public:
	virtual void InitFromDataTable(const class UDataTable* DataTable, FName PrefabName) override;

	virtual void InitFromRegistry(const FName RegistryType, FName PrefabName) override;

	virtual FName GetRegistryTypeName() override;

public:
	UFUNCTION(BlueprintCallable)
	void SpawnEntity();

	UFUNCTION(BlueprintCallable)
	void DestroyEntity();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE AActor* GetEntity() { return Entity; };

public:
	UPROPERTY(BlueprintReadOnly)
	FPropertyFragmentEntityLink PropertyFragment;

private:
	AActor* Entity;
};