// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemPropertyFragment.h"
#include "PropertyFragment_Equipment.generated.h"

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

USTRUCT(BlueprintType)
struct FEquipMode
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ModeTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName AttachSocketName;
};

USTRUCT(BlueprintType, meta = (DisplayName = "Equipment"))
struct FPropertyFragmentEquipment : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FEquipMode> EquipModes;
};

/**
 * 
 */
UCLASS()
class ANCIENTBATTLEFIELD_API UPropertyFragment_Equipment : public UItemPropertyFragment
{
	GENERATED_BODY()

public:
	virtual void Init(const FName Template) override;

	virtual void InitFromRegistry(FName Template) override;

	virtual FGameplayTag GetPropertyTag() override;

	virtual FName GetRegistryTypeName() override;

	virtual FGameplayTagContainer GetRequiredTags() override;

public:
	UFUNCTION(BlueprintCallable)
	void PutOn();

	UFUNCTION(BlueprintCallable)
	void TakeOff();

	UFUNCTION(BlueprintCallable)
	void SwitchMode();

protected:
	UFUNCTION()
	void OnInventoryUpdate(UInventoryComponent* Inventory, int32 LocalID);

	UFUNCTION()
	void OnSpawnEntity(AActor* Entity);

public:
	UPROPERTY(BlueprintReadOnly)
	FPropertyFragmentEquipment PropertyFragment;

private:
	UMeshComponent* ParentMesh;

	TArray<FEquipMode>::TIterator EquipMode = PropertyFragment.EquipModes.CreateIterator();

public:
	static FGameplayTag PropertyTag;

	static FName RegistryType;
};
