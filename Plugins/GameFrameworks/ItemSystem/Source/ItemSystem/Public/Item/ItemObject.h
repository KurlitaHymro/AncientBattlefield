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
	FName PropertyTemplate;
};

USTRUCT(BlueprintType, meta = (DisplayName = "ItemAbstract"))
struct FItemAbstract : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<FPropertyFragmentConfig> PropertyFragments;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer ItemTagContainer;
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ITEMSYSTEM_API UItemObject : public UObject, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	UFUNCTION(BlueprintCallable)
	virtual void AddPropertyFragment(UItemPropertyFragment* PropertyFragmentObject, FName TemplateName = TEXT(""));

	UFUNCTION(BlueprintCallable)
	virtual void RemovePropertyFragment(UItemPropertyFragment* PropertyFragmentObject);

	UFUNCTION(BlueprintCallable)
	class UItemPropertyFragment* FindPropertyFragment(TSubclassOf<UItemPropertyFragment> PropertyFragmentType);
	template<class PropertyFragmentType>
	PropertyFragmentType* FindPropertyFragment()
	{
		return Cast<PropertyFragmentType>(FindPropertyFragment(PropertyFragmentType::StaticClass()));
	};

	UFUNCTION(BlueprintCallable)
	static class UItemObject* NewItemByRegistry(UObject* Outer, FName TemplateName);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tags, meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer ItemTagContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Property,  meta = (AllowPrivateAccess = "true"))
	TArray<class UItemPropertyFragment*> PropertyFragments;

public:
	static FName RegistryType;
};
