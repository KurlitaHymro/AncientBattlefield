// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemPropertyFragment.h"
#include "GameFramework/Actor.h"
#include "PropertyFragment_EntityLink.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemSpawnEntityDelegate, AActor*, Entity);

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
	virtual void InitFromRegistry(FName Template) override;

	virtual FGameplayTag GetPropertyTag() override;

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

	UPROPERTY(BlueprintAssignable)
	FItemSpawnEntityDelegate ItemSpawnEntityDelegate;

private:
	AEntityActor* Entity;

public:
	static FGameplayTag PropertyTag;

	static FName RegistryType;
};

UCLASS()
class ITEMINVENTORYSYSTEM_API AEntityActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEntityActor();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = ())
	class UItemObject* ItemObject;
};