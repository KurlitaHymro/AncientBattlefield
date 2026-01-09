// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySubsystem/Components/InventoryComponent.h"
#include "GameplayTagContainer.h"
#include "FixedSlotsInventoryComponent.generated.h"

USTRUCT(BlueprintType, meta = (DisplayName = "InventorySlot"))
struct FFixedInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Tags)
	FGameplayTagContainer ItemRequiredTags;

	UPROPERTY(EditDefaultsOnly, Category = Tags)
	FGameplayTagContainer ItemBlockedTags;

	UPROPERTY()
	TObjectPtr<class UItemObject> ItemObject;

};

/**
 * <Slots-Array-Inventory>
 */
UCLASS(BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ITEMSYSTEM_API UFixedSlotsInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()
	
public:
	UFixedSlotsInventoryComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PostLoad() override;

public:
	virtual void Add(UItemObject* Item) override;

	virtual void Remove(UItemObject* Item) override;

	virtual UItemObject* FindItemByProperty(TSubclassOf<UItemPropertyFragment> PropertyFragmentType, FName Key);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	FORCEINLINE bool CanAddTo(int32 SlotIndex, UItemObject* Item);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual void AddTo(int32 SlotIndex, UItemObject* Item);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual void RemoveFrom(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual UItemObject* Get(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual int32 FindAvailableSlot(UItemObject* Item);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual bool CanAccommodate(int32 SlotIndex, UItemObject* Item);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual void AddSlot(FGameplayTagContainer ItemRequiredTags, FGameplayTagContainer ItemBlockedTags);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual void RemoveSlot(FGameplayTagContainer ItemRequiredTags, FGameplayTagContainer ItemBlockedTags);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = InventorySlotsSettings, meta = (Tooltip = "Generate slots according to this."))
	int32 SlotCount;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = InventorySlotsSettings, meta = (Tooltip = "Generate slots according to this."))
	TArray<FFixedInventorySlot> FixedSlots;

protected:
	friend class USlotWidget;
	friend class USlotsGridInventoryWidget;
	friend class UItemWidget;
};
