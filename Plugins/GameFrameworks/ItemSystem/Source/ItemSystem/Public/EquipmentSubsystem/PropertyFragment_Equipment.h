// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemPropertyFragment.h"
#include "PropertyFragment_Equipment.generated.h"

USTRUCT(BlueprintType, meta = (DisplayName = "Equipment"))
struct FPropertyFragmentEquipment : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag EquipmentTypeTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName AttachSocketName;
};

/**
 * 
 */
UCLASS()
class ITEMSYSTEM_API UPropertyFragment_Equipment : public UItemPropertyFragment
{
	GENERATED_BODY()
	
public:
	virtual void Setup(const FName Template = "") override;

	virtual void Teardown() override;

	virtual void InitFromRegistry(FName Template) override;

	virtual bool IsKeyMatch(FName Key) override;

	virtual FGameplayTag GetPropertyTag() override;

	virtual FName GetRegistryTypeName() override;

	virtual FGameplayTagContainer GetRequiredTags() override;

public:
	UFUNCTION(BlueprintCallable, Category = Operation, meta = (DisplayName = "Equip"))
	void Equip();

	UFUNCTION(BlueprintCallable, Category = Operation, meta = (DisplayName = "Unequip"))
	void Unequip();

protected:
	UFUNCTION()
	virtual void OnInventoryUpdate(UInventoryComponent* InventoryComponent, int32 Special);

	void SpawnAttachEquipmentEntity(UPrimitiveComponent* Parent = nullptr);

	void DestroyEquipmentEntity();

public:
	UPROPERTY(BlueprintReadOnly)
	FPropertyFragmentEquipment PropertyFragment;

public:
	static FName RegistryType;
};
