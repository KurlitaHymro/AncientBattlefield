// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction_AddToWorld.h"
#include "GameFeatureAction_AddItem.generated.h"

USTRUCT()
struct FInventoryItemsEntry
{
	GENERATED_BODY()

	// The base actor class to add to
	UPROPERTY(EditAnywhere, Category = "Item")
	TSoftClassPtr<AActor> ActorClass;

	// List of Items to give to actors of the specified class
	UPROPERTY(EditAnywhere, Category = "Item")
	TArray<FName> GivedItems;

};

/**
 * 
 */
UCLASS(MinimalAPI, meta = (DisplayName = "Add Inventory Items"))
class UGameFeatureAction_AddItem final : public UGameFeatureAction_AddToWorld
{
	GENERATED_BODY()

	/**  */
	UPROPERTY(EditAnywhere, Category = "Item", meta = (TitleProperty = "ActorClass", ShowOnlyInnerProperties))
	TArray<FInventoryItemsEntry> EntriesList;

private:
	//~ Begin UGameFeatureAction_WorldActionBase interface
	virtual void AddToWorld(const FWorldContext& WorldContext) override;
	//~ End UGameFeatureAction_WorldActionBase interface

	void Reset();
	void HandleActorExtension(AActor* Actor, FName EventName, int32 EntryIndex);
	void AddItems(AActor* Actor, const FInventoryItemsEntry& Entry);
	void RemoveItems(class UInventoryComponent* InventoryComponent);

private:
	class UInventoryComponent* GetActorInventoryComponent(AActor* Actor);

private:
	TMap<class UInventoryComponent*, TArray<class UItemObject*>> ActiveExtensions;
};
