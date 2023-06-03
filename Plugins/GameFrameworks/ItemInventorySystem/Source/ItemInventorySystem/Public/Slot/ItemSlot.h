// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemSlot.generated.h"

USTRUCT()
struct ITEMINVENTORYSYSTEM_API FItemSlot
{
	GENERATED_BODY()

	FItemSlot() { };

	FItemSlot(int32 SlotID);

	UPROPERTY()
	int32 SlotID;

	UPROPERTY()
	class UItemObject* Item;

	UPROPERTY()
	int32 Number;
};

USTRUCT()
struct ITEMINVENTORYSYSTEM_API FItemSlotSet
{
	GENERATED_BODY()

	FItemSlotSet() : Owner(nullptr) { };

	FItemSlotSet(class UInventoryComponent* Owner, int32 SlotsNumber);

private:
	UPROPERTY()
	TArray<FItemSlot> ItemSlots;

	UPROPERTY()
	TObjectPtr<class UInventoryComponent> Owner;

public:
	FItemSlotHandle AddItem(class UItemObject* Item);

	bool RemoveItem(FItemSlotHandle Handle);

	UItemObject* FindItem(FItemSlotHandle Handle);
};

USTRUCT()
struct ITEMINVENTORYSYSTEM_API FItemSlotHandle
{
	GENERATED_BODY()

	UPROPERTY()
	int32 SlotID;

	UPROPERTY()
	TObjectPtr<class UInventoryComponent> Owner;

public:
	bool IsValid();
};


/**
 * 
 */

