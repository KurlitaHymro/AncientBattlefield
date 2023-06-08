// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemSlot.generated.h"

/**
 * һ�����Ӿ��Ǳ���һ���������á�
 * ����ǿɶѵ�����Ʒ��Ӧ���ɶѵ�����Ƭ����ά����ѵ����Լ���ѵ�����ֹ��ܡ�
 * ������������Ƭ���漰��Ʒ���Եı仯����Ȼ��ɶѵ���ͻ��
 * ���ϵͳ����Ľӿ�ֻ����ɾ�Ĳ���Ʒ�Ĵ���
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