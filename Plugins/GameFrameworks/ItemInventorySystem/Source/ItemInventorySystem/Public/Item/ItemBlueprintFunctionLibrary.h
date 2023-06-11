// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ItemBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ITEMINVENTORYSYSTEM_API UItemBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	static class UItemObject* CreateItemObject();

};
