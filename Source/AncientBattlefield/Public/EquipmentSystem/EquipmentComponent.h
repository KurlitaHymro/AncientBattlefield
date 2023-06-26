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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryPutOnDelegate, UItemObject*, Item, EEquipmentSlots, Slot);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryTakeOffDelegate, UItemObject*, Item, EEquipmentSlots, Slot);

/**
 * 
 */
UCLASS()
class ANCIENTBATTLEFIELD_API UEquipmentComponent : public UInventoryComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FInventoryPutOnDelegate OnPutOnEquipment;

	UPROPERTY(BlueprintAssignable)
	FInventoryTakeOffDelegate OnTakeOffEquipment;

public:
	UFUNCTION(BlueprintCallable)
	void EquipmentSetup();

	UFUNCTION(BlueprintCallable)
	void PutOnEquipment(class UItemObject* Equipment, EEquipmentSlots Slot);

	UFUNCTION(BlueprintCallable)
	void TakeOffEquipment(class UItemObject* Equipment);

	UFUNCTION(BlueprintCallable)
	void TakeOffEquipmentFromSlot(EEquipmentSlots Slot);

	UFUNCTION(BlueprintCallable)
	class UItemObject* GetEquipment(EEquipmentSlots Slot);

};
