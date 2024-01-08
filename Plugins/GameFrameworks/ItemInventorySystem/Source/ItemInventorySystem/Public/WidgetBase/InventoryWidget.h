// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

USTRUCT(BlueprintType, meta = (DisplayName = "WidgetSubregio"))
struct FInventoryWidgetSubregioInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Instanced)
	TObjectPtr<class UPanelWidget> Panel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class USlotWidget> SlotWidgetType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UItemWidget> ItemWidgetType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 SubregioBegin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 SubregioSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 SubregioGridPerRow;
};

/**
 * 
 */
UCLASS()
class ITEMINVENTORYSYSTEM_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InstanceEditable = true, ExposeOnSpawn = true))
	class UInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInventoryWidgetSubregioInfo> SubregioConfig;

protected:
	virtual void OnWidgetRebuilt() override;

	UFUNCTION()
	void OnAddItem(class UItemObject* Item, int32 SlotID);

	UFUNCTION()
	void OnRemoveItem(class UItemObject* Item, int32 SlotID);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void GetSubregioConfig();

private:
	void LoadSubregio(FInventoryWidgetSubregioInfo& Subregio);

	int32 AccumulativeSize;

	TMap<int32, UItemWidget*> ItemsWidget;
};
