// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemPropertyFragment.h"
#include "GameFramework/Actor.h"
#include "PropertyFragment_EntityLink.generated.h"

USTRUCT(BlueprintType, meta = (DisplayName = "EntityLink"))
struct FPropertyFragmentEntityLink : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftClassPtr<AEntityActor> EntityType;
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
	FORCEINLINE AEntityActor* GetEntity() { return Entity; };

public:
	UPROPERTY(BlueprintReadOnly)
	FPropertyFragmentEntityLink PropertyFragment;

private:
	AEntityActor* Entity;
};

UCLASS()
class ITEMINVENTORYSYSTEM_API AEntityActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEntityActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (InstanceEditable = true, ExposeOnSpawn = true))
	class UItemObject* ItemObject;
};