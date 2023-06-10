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

	virtual void AddPropertyFragment(TSubclassOf<class UItemPropertyFragment> PropertyFragmentClass);

	template<class PropertyFragmentType>
	const PropertyFragmentType* FindPropertyFragment() const
	{
		return Cast<PropertyFragmentType>(FindPropertyFragment(PropertyFragmentType::StaticClass()));
	};
	class UItemPropertyFragment* FindPropertyFragment(TSubclassOf<class UItemPropertyFragment> PropertyFragmentType);

protected:
	UPROPERTY(EditDefaultsOnly, Category = Tags)
	FGameplayTagContainer ItemTagContainer;

	UPROPERTY()
	TArray<class UItemPropertyFragment*> PropertyFragments;
};
