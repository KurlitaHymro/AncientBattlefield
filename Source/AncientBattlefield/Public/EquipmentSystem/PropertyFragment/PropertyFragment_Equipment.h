// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemPropertyFragment.h"
#include "EquipmentSystem/EquipmentComponent.h"
#include "PropertyFragment_Equipment.generated.h"

/**
 * 
 */
UCLASS()
class ANCIENTBATTLEFIELD_API UPropertyFragment_Equipment : public UItemPropertyFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMeshComponent* ParentMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEquipmentSlots DefaultSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AttachSocket;

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
	UMeshComponent* GetMesh();

private:
	UMeshComponent* EquipmentMesh;
};
