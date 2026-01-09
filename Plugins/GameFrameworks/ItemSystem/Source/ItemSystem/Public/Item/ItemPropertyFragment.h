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
class ITEMSYSTEM_API UItemPropertyFragment : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void Setup(const FName Template = "");

	UFUNCTION(BlueprintCallable)
	virtual void Teardown();

	UFUNCTION(BlueprintCallable)
	virtual void InitFromRegistry(FName Template);

	UFUNCTION(BlueprintCallable)
	virtual bool IsReadyToAdded(UItemObject* Item);

	UFUNCTION(BlueprintCallable)
	virtual bool IsKeyMatch(FName Key);

	UFUNCTION(BlueprintCallable)
	virtual FGameplayTag GetPropertyTag();

	UFUNCTION(BlueprintCallable)
	virtual FName GetRegistryTypeName();

	UFUNCTION(BlueprintCallable)
	virtual FString GetPropertyDescription();

	UFUNCTION(BlueprintCallable)
	virtual FGameplayTagContainer GetRequiredTags();

	UFUNCTION(BlueprintCallable)
	virtual FGameplayTagContainer GetBlockedTags();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InstanceEditable = true, ExposeOnSpawn = true))
	class UItemObject* OwnerItem;
};
