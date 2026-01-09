// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemPropertyFragment.h"
#include "PropertyFragment_Inventory.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FItemPropertyInventoryUpdateDelegate, UInventoryComponent*, Inventory, int32, Special);

DECLARE_DYNAMIC_DELEGATE(FItemInventoryOperationDelegate);

USTRUCT(BlueprintType)
struct FInventoryOperation
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FText ShowText;

	UPROPERTY(BlueprintReadOnly)
	FItemInventoryOperationDelegate CallbackDelegate;

};

/**
 * 
 */
UCLASS()
class ITEMSYSTEM_API UPropertyFragment_Inventory : public UItemPropertyFragment
{
	GENERATED_BODY()

public:
	virtual void Setup(const FName Template = "") override;

	virtual void Teardown() override;

	virtual FGameplayTag GetPropertyTag() override;

	virtual FName GetRegistryTypeName() override;

public:
	UFUNCTION(BlueprintCallable)
	virtual FString GetDebugInformation() const;

	UFUNCTION(BlueprintCallable)
	virtual void RegisterInventoryOperation(int32 Priority, UItemPropertyFragment* InProperty, const FName& CallbackFunctionName, const FText& ShowText);

	UFUNCTION(BlueprintCallable)
	virtual void UnregisterInventoryOperation(int32 Priority);

protected:
	UFUNCTION()
	virtual void OnInventoryUpdate(UInventoryComponent* InventoryComponent, int32 Special);

public:
	UPROPERTY(BlueprintAssignable)
	FItemPropertyInventoryUpdateDelegate InventoryUpdateDelegate;

	UPROPERTY(BlueprintReadOnly)
	UInventoryComponent* Inventory;

	UPROPERTY(BlueprintReadOnly)
	int32 SpecialValue;

	UPROPERTY(BlueprintReadOnly)
	TMap<int32, FInventoryOperation> PriorityInventoryOperations;

public:
	static FName RegistryType;

};
