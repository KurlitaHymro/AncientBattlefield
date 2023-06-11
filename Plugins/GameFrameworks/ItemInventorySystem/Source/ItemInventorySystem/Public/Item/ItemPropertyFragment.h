// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "ItemPropertyFragment.generated.h"

USTRUCT(BlueprintType)
struct ITEMINVENTORYSYSTEM_API FPropertyFragmentInit
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class UItemPropertyFragment> PropertyFragmentType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UDataTable* DataTable;

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
	virtual void OnItemInstantiate(class UItemObject* Owner) const;

	virtual void InitFromMetaDataTable(const class UDataTable* DataTable, FString PrefabName);

	virtual void OnRefresh() const;
};
