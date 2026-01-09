// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Interface.h"
#include "InventorySystemInterface.generated.h"

class UInventoryComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UInventorySystemInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class ITEMSYSTEM_API IInventorySystemInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	/** Returns the inventory component to use for this actor. It may live on another actor. */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	virtual UInventoryComponent* GetInventoryComponent(FName Key = "") const = 0;
};