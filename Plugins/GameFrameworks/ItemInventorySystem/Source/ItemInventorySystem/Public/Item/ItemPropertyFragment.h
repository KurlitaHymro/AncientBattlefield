// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "ItemPropertyFragment.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ITEMINVENTORYSYSTEM_API UItemPropertyFragment : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void InitFromDataTable(const UDataTable* DataTable, FName PrefabName);

	UFUNCTION(BlueprintCallable)
	virtual void InitFromRegistry(const FName RegistryType, FName PrefabName);

	UFUNCTION(BlueprintCallable)
	virtual FName GetRegistryTypeName();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InstanceEditable = true, ExposeOnSpawn = true))
	class UItemObject* Owner;
};
