// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "EquipmentComponent.generated.h"

UENUM(BlueprintType)
enum class EEquipmentSlots : uint8
{
	Helmet,
	Breastplate,
	Glove,
	Belt,
	Shoes,
	Cloak,

	MainHand,
	OffHand,

	EquipmentSlotsNum
};

/**
 * 
 */
UCLASS()
class ANCIENTBATTLEFIELD_API UEquipmentComponent : public UInventoryComponent
{
	GENERATED_BODY()

public:
	virtual void Setup(int32 SlotsNumber) override;

	virtual void AddItem(UItemObject* Item, int32 SlotID) override;

	virtual void RemoveItem(UItemObject* Item) override;

	virtual bool CanMoveTo(UItemObject* Item, int32 SlotID) const override;

public:
	UFUNCTION(BlueprintCallable)
	UItemObject* GetEquipment(EEquipmentSlots Slot);
};
