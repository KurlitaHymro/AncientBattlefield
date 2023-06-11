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

protected:
	virtual void OnItemInstantiate(class UItemObject* Owner) const override;

	virtual void InitFromMetaDataTable(const class UDataTable* DataTable, FString PrefabName) override;

	virtual void OnRefresh() const override;

public:
	UPROPERTY(BlueprintReadOnly)
	FPropertyFragmentPropBaseInfo PropertyFragment;

};