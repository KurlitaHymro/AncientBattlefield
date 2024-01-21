// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryAddItemDelegate, UItemObject*, Item, int32, LocalID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryRemoveItemDelegate, UItemObject*, Item, int32, LocalID);

USTRUCT(BlueprintType, meta = (DisplayName = "InventorySlot"))
struct FInventorySlot
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Tags)
	FGameplayTagContainer ItemBlockedTags;

	UPROPERTY(EditDefaultsOnly, Category = Tags)
	FGameplayTagContainer ItemRequiredTags;

	UPROPERTY()
	TObjectPtr<UItemObject> Item;

	bool CanHold(UItemObject* ExternItem) const;
};

UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ITEMINVENTORYSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UFUNCTION(BlueprintCallable)
	virtual UItemObject* GetItem(int32 SlotID);

	UFUNCTION(BlueprintCallable)
	virtual bool CanHold(UItemObject* Item, int32 SlotID) const;

	UFUNCTION(BlueprintCallable)
	virtual bool AddItem(UItemObject* Item);

	UFUNCTION(BlueprintCallable)
	virtual bool AddItemToSlot(UItemObject* Item, int32 SlotID);

	UFUNCTION(BlueprintCallable)
	virtual bool RemoveItem(UItemObject* Item);

	UFUNCTION(BlueprintCallable)
	virtual bool RemoveItemFromSlot(int32 SlotID);

	UFUNCTION(BlueprintCallable)
	virtual bool SwapItems(UItemObject* Item1, UItemObject* Item2);

	UFUNCTION(BlueprintCallable)
	virtual bool CollectItems();

	UFUNCTION(BlueprintCallable)
	virtual FString GetStaticDescription() const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", InstanceEditable = true))
	TArray<FInventorySlot> Slots;

public:
	UPROPERTY(BlueprintAssignable, meta = (DisplayName = OnInventoryAddItem))
	FInventoryAddItemDelegate InventoryAddItemDelegate;

	UPROPERTY(BlueprintAssignable, meta = (DisplayName = OnInventoryRemoveItem))
	FInventoryRemoveItemDelegate InventoryRemoveItemDelegate;

private:
	static const int32 InvalidIndex;
};
