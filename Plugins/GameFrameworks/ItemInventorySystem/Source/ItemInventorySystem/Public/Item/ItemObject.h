// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagAssetInterface.h"
#include "ItemObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAddToInventoryDelegate, UInventoryComponent*, InventoryComponent);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRemoveFromInventoryDelegate, UInventoryComponent*, InventoryComponent);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ITEMINVENTORYSYSTEM_API UItemObject : public UObject, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FAddToInventoryDelegate OnAddToInventory;

	UPROPERTY(BlueprintAssignable)
	FRemoveFromInventoryDelegate OnRemoveFromInventory;

public:
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	UFUNCTION(BlueprintCallable)
	virtual void AddPropertyFragment(UItemPropertyFragment* PropertyFragmentObject);

	UFUNCTION(BlueprintCallable)
	class UItemPropertyFragment* FindPropertyFragment(TSubclassOf<UItemPropertyFragment> PropertyFragmentType);
	template<class PropertyFragmentType>
	PropertyFragmentType* FindPropertyFragment()
	{
		return Cast<PropertyFragmentType>(FindPropertyFragment(PropertyFragmentType::StaticClass()));
	};

protected:
	UPROPERTY(EditDefaultsOnly, Category = Tags)
	FGameplayTagContainer ItemTagContainer;

	UPROPERTY()
	TArray<class UItemPropertyFragment*> PropertyFragments;

};
