// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "InventoryItemConfig.generated.h"

USTRUCT(BlueprintType, meta = (DisplayName = "AncientBattlefieldItem"))
struct FAncientBattlefieldItem : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	bool bPropBaseInfo;

	UPROPERTY(EditDefaultsOnly)
	FName PropBaseInfoPrefab;
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
	static class UItemObject* NewInventoryItem(FName PrefabName);
	
};
