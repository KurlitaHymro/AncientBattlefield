// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventorySetupDelegate, int32, SlotsNumber);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryAddItemDelegate, UItemObject*, Item, int32, LocalID);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryRemoveItemDelegate, UItemObject*, Item, int32, LocalID);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ITEMINVENTORYSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

public:
	UFUNCTION(BlueprintCallable)
	void Setup(int32 SlotsNumber);

	UFUNCTION(BlueprintCallable)
	int32 FindVacancy() const;

	UFUNCTION(BlueprintCallable)
	void AddItem(UItemObject* Item, int32 SlotID);

	UFUNCTION(BlueprintCallable)
	void RemoveItem(UItemObject* Item);

	UFUNCTION(BlueprintCallable)
	void RemoveItemFromSlot(int32 SlotID);

	UFUNCTION(BlueprintCallable)
	UItemObject* GetItem(int32 SlotID);

	UFUNCTION(BlueprintCallable)
	int32 GetSize();

protected:
	UPROPERTY()
	TArray<UItemObject*> ItemObjectSlot;

	UPROPERTY()
	int32 Size;

public:
	UPROPERTY(BlueprintAssignable)
	FInventorySetupDelegate OnSetup;

	UPROPERTY(BlueprintAssignable)
	FInventoryAddItemDelegate OnAddItem;

	UPROPERTY(BlueprintAssignable)
	FInventoryRemoveItemDelegate OnRemoveItem;

};
