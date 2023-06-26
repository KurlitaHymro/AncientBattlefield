// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemPropertyFragment.h"
#include "EquipmentSystem/EquipmentComponent.h"
#include "PropertyFragment_Equipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentPutOnDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentTakeOffDelegate);

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
	FEquipmentPutOnDelegate OnEquipmentPutOn;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FEquipmentTakeOffDelegate OnEquipmentTakeOff;

protected:
	virtual void Instantiate(class UItemObject* Owner) override;

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
