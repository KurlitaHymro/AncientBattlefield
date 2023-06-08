// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemSlot.generated.h"

/**
 * 一个格子就是保存一个对象引用。
 * 如果是可堆叠的物品，应该由堆叠属性片段来维护其堆叠数以及其堆叠、拆分功能。
 * 特例化类属性片段涉及物品属性的变化，必然与可堆叠冲突。
 * 库存系统对外的接口只有增删改查物品的代理。
 */
USTRUCT()
struct ITEMINVENTORYSYSTEM_API FItemSlot
{
	GENERATED_BODY()

	UPROPERTY()
	class UItemObject* Item;
};

USTRUCT()
struct ITEMINVENTORYSYSTEM_API FItemSlotSet
{
	GENERATED_BODY()

private:
	UPROPERTY()
	int32 Size;

	UPROPERTY()
	TObjectPtr<class UInventoryComponent> Owner;

	UPROPERTY()
	TArray<FItemSlot> ItemSlots;

public:
	void Init(class UInventoryComponent* OwnerInventoryComponent, int32 SlotsNumber);

	FItemSlotHandle AddItem(class UItemObject* Item);

	bool RemoveItem(FItemSlotHandle Handle);

	UItemObject* FindItem(FItemSlotHandle Handle);
};

USTRUCT(BlueprintType)
struct ITEMINVENTORYSYSTEM_API FItemSlotHandle
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<class UInventoryComponent> Owner;

	UPROPERTY()
	int32 SlotID;

public:
	bool IsValid();
};