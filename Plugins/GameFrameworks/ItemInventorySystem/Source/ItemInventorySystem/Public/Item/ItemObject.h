// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagAssetInterface.h"
#include "ItemObject.generated.h"

/**
 * 
 */
UCLASS()
class ITEMINVENTORYSYSTEM_API UItemObject : public UObject, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	template<class PropertyFragmentType>
	PropertyFragmentType* FindPropertyFragment()
	{
		return Cast<PropertyFragmentType>(FindPropertyFragment(PropertyFragmentType::StaticClass()));
	};

	UFUNCTION(BlueprintCallable)
	class UItemPropertyFragment* FindPropertyFragment(UClass* PropertyFragmentType);

	UFUNCTION(BlueprintCallable)
	virtual void AddPropertyFragment(TSubclassOf<UItemPropertyFragment> PropertyFragmentType, UDataTable* DataTable = nullptr, FString PropertyFragmentPrefabName = "");

protected:
	UPROPERTY(EditDefaultsOnly, Category = Tags)
	FGameplayTagContainer ItemTagContainer;

	UPROPERTY()
	TArray<class UItemPropertyFragment*> PropertyFragments;

};
