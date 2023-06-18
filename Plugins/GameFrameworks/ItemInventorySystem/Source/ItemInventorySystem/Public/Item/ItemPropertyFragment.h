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
	UFUNCTION()
	UItemObject* GetOwner() const;

public:
	virtual void Instantiate(class UItemObject* Owner);

	UFUNCTION(BlueprintCallable)
	virtual void InitFromMetaDataTable(const class UDataTable* DataTable, FString PrefabName);

private:
	UItemObject* OwnerItemObject;
};
