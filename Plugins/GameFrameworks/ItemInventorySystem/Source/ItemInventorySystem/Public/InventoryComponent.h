// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Slot/ItemSlot.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInitSlotsDelegate, int32, SlotsNumber);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAddItemDelegate, UItemObject*, Item, int32, LocalID);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRemoveItemDelegate, UItemObject*, Item, int32, LocalID);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ITEMINVENTORYSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

public:
	UFUNCTION(BlueprintCallable)
	void InitSlots(int32 SlotsNumber);

	UFUNCTION(BlueprintCallable)
	FItemSlotHandle AddItem(UItemObject* Item);

	UFUNCTION(BlueprintCallable)
	bool RemoveItem(FItemSlotHandle SlotHandle);

private:
	UPROPERTY()
	FItemSlotSet SlotSet;

public:
	UPROPERTY(BlueprintAssignable)
	FInitSlotsDelegate OnInitSlot;

	UPROPERTY(BlueprintAssignable)
	FAddItemDelegate OnAddItem;

	UPROPERTY(BlueprintAssignable)
	FRemoveItemDelegate OnRemoveItem;

};
