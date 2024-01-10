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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UItemObject> Item;
};

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
	virtual int32 FindVacancy(UItemObject* Item) const;

	UFUNCTION(BlueprintCallable)
	virtual bool CanHold(UItemObject* Item, int32 SlotID) const;

	UFUNCTION(BlueprintCallable)
	virtual void AddItem(UItemObject* Item, int32 SlotID);

	UFUNCTION(BlueprintCallable)
	virtual void RemoveItem(UItemObject* Item);

	UFUNCTION(BlueprintCallable)
	virtual void RemoveItemFromSlot(int32 SlotID);

	UFUNCTION(BlueprintCallable)
	virtual void AppendSlotTags(int32 SlotID, FGameplayTagContainer BlockedTags, FGameplayTagContainer RequiredTags);

	UFUNCTION(BlueprintCallable)
	virtual UItemObject* SwapItem(int32 SlotID, UItemObject* OtherItem);

	UFUNCTION(BlueprintCallable)
	virtual UItemObject* GetItem(int32 SlotID);

	UFUNCTION(BlueprintCallable)
	virtual bool CollectToUniversalSlots();

	UFUNCTION(BlueprintCallable)
	virtual FString GetStaticDescription() const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<FInventorySlot> Slots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", UIMin = "0", AllowPrivateAccess = "true", InstanceEditable = true, ExposeOnSpawn = true))
	int32 Size;

public:
	UPROPERTY(BlueprintAssignable)
	FInventoryAddItemDelegate InventoryAddItemDelegate;

	UPROPERTY(BlueprintAssignable)
	FInventoryRemoveItemDelegate InventoryRemoveItemDelegate;

};
