// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemPropertyFragment.h"
#include "EquipmentSystem/EquipmentComponent.h"
#include "PropertyFragment_Equipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FItemEquipmentPutOnDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FItemEquipmentTakeOffDelegate);

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
	TArray<EEquipmentSlots> RestrictSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AttachSocket;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FItemEquipmentPutOnDelegate ItemEquipmentPutOnDelegate;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FItemEquipmentTakeOffDelegate ItemEquipmentTakeOffDelegate;

protected:
	virtual void Instantiate(class UItemObject* Owner) override;

	UFUNCTION()
	void OnEquipmentPutOn();

	UFUNCTION()
	void OnEquipmentTakeOff();

public:
	UFUNCTION(BlueprintCallable)
	void PutOn();

	UFUNCTION(BlueprintCallable)
	void TakeOff();

	UFUNCTION(BlueprintCallable)
	UMeshComponent* GetMesh();

private:
	AActor* EquipmentEntity;

	UMeshComponent* EquipmentMesh;
};
