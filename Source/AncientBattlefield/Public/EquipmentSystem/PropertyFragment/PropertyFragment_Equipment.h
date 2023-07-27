// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemPropertyFragment.h"
#include "EquipmentSystem/EquipmentComponent.h"
#include "PropertyFragment_Equipment.generated.h"

USTRUCT(BlueprintType, meta = (DisplayName = "Equipment"))
struct FPropertyFragmentEquipment : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<EEquipmentSlots, FName> EquipmentSlots;
};

/**
 * 
 */
UCLASS()
class ANCIENTBATTLEFIELD_API UPropertyFragment_Equipment : public UItemPropertyFragment
{
	GENERATED_BODY()

public:
	virtual void InitFromDataTable(const class UDataTable* DataTable, FName PrefabName) override;

	virtual void InitFromRegistry(const FName RegistryType, FName PrefabName) override;

	virtual FName GetRegistryTypeName() override;

public:
	UFUNCTION(BlueprintCallable)
	void PutOn();

	UFUNCTION(BlueprintCallable)
	void TakeOff();

	UFUNCTION()
	void OnEquipmentPutOn(UMeshComponent* TargetMesh, EEquipmentSlots TargetSlot);

	UFUNCTION()
	void OnEquipmentTakeOff();

public:
	UPROPERTY(BlueprintReadOnly)
	FPropertyFragmentEquipment PropertyFragment;

private:
	UMeshComponent* ParentMesh;
};
