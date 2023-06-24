// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemPropertyFragment.h"
#include "EquipmentSystem/EquipmentComponent.h"
#include "PropertyFragment_MeleeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ANCIENTBATTLEFIELD_API UPropertyFragment_MeleeWeapon : public UItemPropertyFragment
{
	GENERATED_BODY()

public:


protected:
	virtual void Instantiate(class UItemObject* Owner) override;

	UFUNCTION()
	void OnAddToInventoryComponent(UInventoryComponent* InventoryComponent);

public:
	UFUNCTION(BlueprintCallable)
	void PutOn(EEquipmentSlots Slot, UItemObject* Item);

	UFUNCTION(BlueprintCallable)
	void TakeOff(EEquipmentSlots Slot, UItemObject* Item);

	UFUNCTION(BlueprintCallable)
	class UHitTraceComponent* GetHitTraceComponent();

private:
	class UHitTraceComponent* HitTraceComponent;

	class UMeshComponent* Mesh;
};
