// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "ItemPropertyFragment.generated.h"

/**
 * 
 */
UCLASS(abstract, BlueprintType, Blueprintable)
class ITEMINVENTORYSYSTEM_API UItemPropertyFragment : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void Init(const FName Template = "");

	UFUNCTION(BlueprintCallable)
	virtual void InitFromRegistry(FName Template);

	UFUNCTION(BlueprintCallable)
	virtual bool IsDependencyReady(UItemObject* Item);

protected:
	UFUNCTION(BlueprintCallable)
	virtual FGameplayTag GetPropertyTag();

	UFUNCTION(BlueprintCallable)
	virtual FName GetRegistryTypeName();

	UFUNCTION(BlueprintCallable)
	virtual FGameplayTagContainer GetRequiredTags();

	UFUNCTION(BlueprintCallable)
	virtual FGameplayTagContainer GetBlockedTags();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InstanceEditable = true, ExposeOnSpawn = true))
	class UItemObject* Owner;
};
