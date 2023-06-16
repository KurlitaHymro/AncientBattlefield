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

protected:

public:
	UFUNCTION(BlueprintCallable)
	void InitEquipmentSlots();

	UFUNCTION(BlueprintCallable)
	bool PutOnEquipment(class UItemObject* Equipment, EEquipmentSlots Slot);

	UFUNCTION(BlueprintCallable)
	bool TakeOffEquipment(EEquipmentSlots Slot);

	UFUNCTION(BlueprintCallable)
	class UItemObject* GetEquipment(EEquipmentSlots Slot);

private:
	TArray<FItemSlotHandle> SlotHandles;

};
