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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPutOnDelegate, EEquipmentSlots, Slot, UItemObject*, Item);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTakeOffDelegate, EEquipmentSlots, Slot, UItemObject*, Item);

/**
 * 
 */
UCLASS()
class ANCIENTBATTLEFIELD_API UEquipmentComponent : public UInventoryComponent
{
	GENERATED_BODY()
	
protected:
	UEquipmentComponent();

public:
	FPutOnDelegate OnPutOnEquipment;

	FTakeOffDelegate OnTakeOffEquipment;

public:
	UFUNCTION(BlueprintCallable)
	void InitEquipmentSlots();

	UFUNCTION(BlueprintCallable)
	bool PutOnEquipment(class UItemObject* Equipment, EEquipmentSlots Slot);

	UFUNCTION(BlueprintCallable)
	bool TakeOffEquipment(EEquipmentSlots Slot);

	UFUNCTION(BlueprintCallable)
	class UItemObject* GetEquipment(EEquipmentSlots Slot);

};
