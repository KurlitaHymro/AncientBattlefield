// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemPropertyFragment.h"
#include "PropertyFragment_EntityLink.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemPropertyEntityLinkSpawnEntityDelegate, AActor*, Entity);

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
class ITEMSYSTEM_API UPropertyFragment_EntityLink : public UItemPropertyFragment
{
	GENERATED_BODY()

public:
	virtual void Setup(const FName Template = "") override;

	virtual void InitFromRegistry(FName Template) override;

	virtual FGameplayTag GetPropertyTag() override;

	virtual FName GetRegistryTypeName() override;

public:
	void SpawnEntity(FVector const Location = FVector::ZeroVector, FRotator const Rotation = FRotator::ZeroRotator);

	void DestroyEntity();

	FORCEINLINE AActor* GetEntity() { return Entity; };

	UFUNCTION(BlueprintCallable, Category = Operation, meta = (DisplayName = "Drop"))
	void Drop();

public:
	UPROPERTY(BlueprintReadOnly)
	FPropertyFragmentEntityLink PropertyFragment;

	UPROPERTY(BlueprintAssignable)
	FItemPropertyEntityLinkSpawnEntityDelegate EntityLinkSpawnEntityDelegate;

private:
	AActor* Entity;

public:
	static FName RegistryType;

};
