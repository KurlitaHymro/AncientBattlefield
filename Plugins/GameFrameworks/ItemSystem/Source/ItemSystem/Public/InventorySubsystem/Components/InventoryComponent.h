// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FInventoryAddItemDelegate, class UItemObject*, int32);
DECLARE_MULTICAST_DELEGATE_TwoParams(FInventoryRemoveItemDelegate, class UItemObject*, int32);

/**
 * <Set-Inventory>
 */
UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ITEMSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual void Add(UItemObject* Item);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual void Remove(UItemObject* Item);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual UItemObject* FindItemByProperty(TSubclassOf<UItemPropertyFragment> PropertyFragmentType, FName Key);

protected:
	friend class UInventoryWidget;
	friend class USlotsGridInventoryWidget;
	friend class USlotsFixedInventoryWidget;

	FInventoryAddItemDelegate InventoryAddItemDelegate;

	FInventoryRemoveItemDelegate InventoryRemoveItemDelegate;

private:
	friend class UItemWidget;

	TSet<TObjectPtr<class UItemObject>> Items;

};
