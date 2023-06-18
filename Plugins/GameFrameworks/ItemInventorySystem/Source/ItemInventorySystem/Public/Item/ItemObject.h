// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagAssetInterface.h"
#include "ItemObject.generated.h"

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
	class UItemPropertyFragment* FindPropertyFragment(TSubclassOf<UItemPropertyFragment> PropertyFragmentType);

	UFUNCTION(BlueprintCallable)
	virtual UItemPropertyFragment* ConstructPropertyFragment(TSubclassOf<UItemPropertyFragment> PropertyFragmentType);

	template<class PropertyFragmentType>
	PropertyFragmentType* FindPropertyFragment()
	{
		return Cast<PropertyFragmentType>(FindPropertyFragment(PropertyFragmentType::StaticClass()));
	};

	template<class PropertyFragmentType>
	PropertyFragmentType* ConstructPropertyFragment()
	{
		return Cast<PropertyFragmentType>(ConstructPropertyFragment(PropertyFragmentType::StaticClass()));
	}

protected:
	UPROPERTY(EditDefaultsOnly, Category = Tags)
	FGameplayTagContainer ItemTagContainer;

	UPROPERTY()
	TArray<class UItemPropertyFragment*> PropertyFragments;

};
