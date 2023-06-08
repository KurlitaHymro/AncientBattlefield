// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Slot/ItemSlot.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAddItemDelegate, UItemObject*, Item);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRemoveItemDelegate, UItemObject*, Item);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ITEMINVENTORYSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for78451 this component's properties
	UInventoryComponent();

public:
	UFUNCTION(BlueprintCallable)
	FItemSlotHandle AddItem(UItemObject* Item);

	UFUNCTION(BlueprintCallable)
	bool RemoveItem(FItemSlotHandle SlotHandle);

private:
	UPROPERTY()
	FItemSlotSet SlotSet;

public:
	UPROPERTY(BlueprintAssignable)
	FAddItemDelegate OnAddItem;

	UPROPERTY(BlueprintAssignable)
	FRemoveItemDelegate OnRemoveItem;

};
