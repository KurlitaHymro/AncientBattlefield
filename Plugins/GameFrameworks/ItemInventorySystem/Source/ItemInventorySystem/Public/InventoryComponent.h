// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventorySetupDelegate, int32, SlotsNumber);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryAddItemDelegate, UItemObject*, Item, int32, LocalID);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryRemoveItemDelegate, UItemObject*, Item, int32, LocalID);

UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ITEMINVENTORYSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PostLoad() override;

public:
	UFUNCTION(BlueprintCallable)
	virtual void Setup(int32 SlotsNumber);

	UFUNCTION(BlueprintCallable)
	virtual int32 FindVacancy() const;

	UFUNCTION(BlueprintCallable)
	virtual bool CanMoveTo(UItemObject* Item, int32 SlotID) const;

	UFUNCTION(BlueprintCallable)
	virtual void AddItem(UItemObject* Item, int32 SlotID);

	UFUNCTION(BlueprintCallable)
	virtual void RemoveItem(UItemObject* Item);

	UFUNCTION(BlueprintCallable)
	virtual void RemoveItemFromSlot(int32 SlotID);

	UFUNCTION(BlueprintCallable)
	virtual UItemObject* SwapItem(int32 SlotID, UItemObject* OtherItem);

	UFUNCTION(BlueprintCallable)
	virtual UItemObject* GetItem(int32 SlotID);

	UFUNCTION(BlueprintCallable)
	virtual int32 GetSize() const;

	UFUNCTION(BlueprintCallable)
	virtual FString GetStaticDescription() const;

protected:
	UPROPERTY()
	TArray<UItemObject*> ItemObjectSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0", InstanceEditable = true, ExposeOnSpawn = true))
	int32 Size;

public:
	UPROPERTY(BlueprintAssignable)
	FInventorySetupDelegate InventorySetupDelegate;

	UPROPERTY(BlueprintAssignable)
	FInventoryAddItemDelegate InventoryAddItemDelegate;

	UPROPERTY(BlueprintAssignable)
	FInventoryRemoveItemDelegate InventoryRemoveItemDelegate;

};
