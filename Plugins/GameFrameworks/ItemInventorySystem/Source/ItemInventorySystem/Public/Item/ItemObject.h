// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagAssetInterface.h"
#include "ItemObject.generated.h"

USTRUCT(BlueprintType, meta = (DisplayName = "ItemPropertyFragment"))
struct FPropertyFragmentConfig
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<class UItemPropertyFragment> PropertyClass;

	UPROPERTY(EditDefaultsOnly)
	FName PropertyPrefab;
};

USTRUCT(BlueprintType, meta = (DisplayName = "ItemAbstract"))
struct FItemAbstract : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<FPropertyFragmentConfig> PropertyFragments;
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ITEMINVENTORYSYSTEM_API UItemObject : public UObject, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

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

	UFUNCTION(BlueprintCallable)
	static class UItemObject* NewItem(UObject* Outer, FName PrefabName);

private:
	UPROPERTY(EditDefaultsOnly, Category = Tags)
	FGameplayTagContainer ItemTagContainer;

	UPROPERTY()
	TArray<class UItemPropertyFragment*> PropertyFragments;

public:
	UPROPERTY(BlueprintReadOnly)
	class UInventoryComponent* BelongingInventory;

	UPROPERTY(BlueprintReadOnly)
	int32 BelongingSlotID;
};
