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
	virtual void Init(UItemObject* Item, const FName PrefabName = "", const FName RegistryType = "", const UDataTable* DataTable = nullptr);

	UFUNCTION(BlueprintCallable)
	virtual void InitFromDataTable(const class UDataTable* DataTable, FName PrefabName);

	UFUNCTION(BlueprintCallable)
	virtual void InitFromRegistry(const FName RegistryType, FName PrefabName);

	UFUNCTION(BlueprintCallable)
	virtual FName GetPropertyTagName();

	UFUNCTION(BlueprintCallable)
	virtual FName GetRegistryTypeName();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InstanceEditable = true, ExposeOnSpawn = true))
	class UItemObject* Owner;
};
