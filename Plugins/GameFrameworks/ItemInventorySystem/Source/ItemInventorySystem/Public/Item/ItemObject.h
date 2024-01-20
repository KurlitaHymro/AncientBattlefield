// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagAssetInterface.h"
#include "ItemObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryUpdateDelegate, UInventoryComponent*, Inventory, int32, LocalID);

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
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class ITEMINVENTORYSYSTEM_API UItemObject : public UObject, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	UItemObject();

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	UFUNCTION(BlueprintCallable)
	virtual void AddPropertyFragment(UItemPropertyFragment* PropertyFragmentObject, FName TemplateName = TEXT(""));

	UFUNCTION(BlueprintCallable)
	class UItemPropertyFragment* FindPropertyFragment(TSubclassOf<UItemPropertyFragment> PropertyFragmentType);
	template<class PropertyFragmentType>
	PropertyFragmentType* FindPropertyFragment()
	{
		return Cast<PropertyFragmentType>(FindPropertyFragment(PropertyFragmentType::StaticClass()));
	};

	UFUNCTION(BlueprintCallable)
	static class UItemObject* NewItemByRegistry(UObject* Outer, FName TemplateName);

protected:
	UFUNCTION()
	void OnInventoryUpdate(UInventoryComponent* Inventory, int32 LocalID);

public:
	UPROPERTY(BlueprintAssignable)
	FInventoryUpdateDelegate InventoryUpdateDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tags, meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer ItemTagContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Property,  meta = (AllowPrivateAccess = "true"))
	TArray<class UItemPropertyFragment*> PropertyFragments;

	UPROPERTY(BlueprintReadOnly)
	class UInventoryComponent* BelongingInventory;

	UPROPERTY(BlueprintReadOnly)
	int32 BelongingSlotID;

public:
	static FName RegistryType;
};
