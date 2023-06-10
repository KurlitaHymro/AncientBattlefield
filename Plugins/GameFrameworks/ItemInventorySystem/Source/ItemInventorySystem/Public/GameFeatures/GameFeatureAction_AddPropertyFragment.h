// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction_AddToWorld.h"
#include "GameFeatureAction_AddPropertyFragment.generated.h"

/**
 *
 */

USTRUCT()
struct FPropertyFragmentEntry
{
	GENERATED_BODY()

	// The base actor class to add to
	UPROPERTY(EditAnywhere, Category = "Item")
	int32 ItemID;

	// List of abilities to grant to actors of the specified class
	UPROPERTY(EditAnywhere, Category = "Item")
	TArray<TSubclassOf<class UItemPropertyFragment>> GrantedPropertyFragments;
};

UCLASS()
class ITEMINVENTORYSYSTEM_API UGameFeatureAction_AddPropertyFragment : public UGameFeatureAction_AddToWorld
{
	GENERATED_BODY()

public:
	
};
