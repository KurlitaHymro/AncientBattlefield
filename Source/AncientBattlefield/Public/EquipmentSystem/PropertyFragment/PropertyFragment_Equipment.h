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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InstanceEditable = true, ExposeOnSpawn = true))
	TArray<EEquipmentSlots> RestrictSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InstanceEditable = true, ExposeOnSpawn = true))
	FName AttachSocket;

protected:


public:
	UFUNCTION(BlueprintCallable)
	void PutOn();

	UFUNCTION(BlueprintCallable)
	void TakeOff();

	UFUNCTION()
	void OnEquipmentPutOn();

	UFUNCTION()
	void OnEquipmentTakeOff();

private:
	AActor* EquipmentEntity;
};
