// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemPropertyFragment.h"
#include "PropertyFragment_PropBaseInfo.generated.h"

USTRUCT(BlueprintType)
struct FPropertyFragmentPropBaseInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FName Name;

	UPROPERTY(BlueprintReadWrite)
	UTexture* Icon;
};

/**
 * 
 */
UCLASS()
class ITEMINVENTORYSYSTEM_API UPropertyFragment_PropBaseInfo : public UItemPropertyFragment
{
	GENERATED_BODY()

protected:
	virtual void OnItemInstantiate() const override;

	virtual void InitFromMetaDataTable(const class UDataTable* DataTable, FString PrefabName) override;

public:
	FPropertyFragmentPropBaseInfo PropertyFragment;
};