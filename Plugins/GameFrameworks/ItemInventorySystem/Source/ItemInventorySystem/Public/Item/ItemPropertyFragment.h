// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemPropertyFragment.generated.h"

USTRUCT(BlueprintType)
struct FPropertyFragmentInit
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class UItemPropertyFragment> PropertyFragmentType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UDataTable* DataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString PropertyFragmentPrefabName;
};

/**
 * 
 */
UCLASS()
class ITEMINVENTORYSYSTEM_API UItemPropertyFragment : public UObject
{
	GENERATED_BODY()

public:
	virtual void OnItemInstantiate() const;

	virtual void InitFromMetaDataTable(const class UDataTable* DataTable, FString PrefabName);
};
