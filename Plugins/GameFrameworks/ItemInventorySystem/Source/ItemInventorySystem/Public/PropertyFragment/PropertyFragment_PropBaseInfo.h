// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemPropertyFragment.h"
#include "PropertyFragment_PropBaseInfo.generated.h"

USTRUCT(BlueprintType, meta = (DisplayName = "PropertyFragment_PropBaseInfo"))
struct ITEMINVENTORYSYSTEM_API FPropertyFragmentPropBaseInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture* Icon;
};

/**
 * 
 */
UCLASS()
class ITEMINVENTORYSYSTEM_API UPropertyFragment_PropBaseInfo : public UItemPropertyFragment
{
	GENERATED_BODY()

public:
	virtual void InitFromDataTable(const class UDataTable* DataTable, FName PrefabName);

	virtual void InitFromRegistry(const FName RegistryType, FName PrefabName);

public:
	UPROPERTY(BlueprintReadOnly)
	FPropertyFragmentPropBaseInfo PropertyFragment;

};