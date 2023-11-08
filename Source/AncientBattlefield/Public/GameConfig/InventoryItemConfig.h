// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "InventoryItemConfig.generated.h"

USTRUCT(BlueprintType, meta = (DisplayName = "PropBaseInfo"))
struct FPropertyFragmentConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<class UItemPropertyFragment> PropertyClass;

	UPROPERTY(EditDefaultsOnly)
	FName PropertyPrefab;
};

USTRUCT(BlueprintType, meta = (DisplayName = "AncientBattlefieldItem"))
struct FAncientBattlefieldItem : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<FPropertyFragmentConfig> PropertyFragments;
};

/**
 * 
 */
UCLASS()
class ANCIENTBATTLEFIELD_API UInventoryItemConfig : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static class UItemObject* NewInventoryItem(UObject* Outer, FName PrefabName);
	
};
