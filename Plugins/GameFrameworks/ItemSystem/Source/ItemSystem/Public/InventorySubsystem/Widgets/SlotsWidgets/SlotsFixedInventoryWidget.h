// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySubsystem/Widgets/InventoryWidget.h"
#include "SlotsFixedInventoryWidget.generated.h"

/**
 * 固定槽位显示方案
 */
UCLASS()
class ITEMSYSTEM_API USlotsFixedInventoryWidget : public UInventoryWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InstanceEditable = true, ExposeOnSpawn = true))
	TSubclassOf<class USlotWidget> SlotWidgetType;

protected:
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void OnAddItem(class UItemObject* Item, int32 SpecialValue) override;

	virtual void OnRemoveItem(class UItemObject* Item, int32 SpecialValue) override;

	UFUNCTION(BlueprintImplementableEvent)
	TArray<USlotWidget*> GetSlotWidgetsArray();

protected:
	class UFixedSlotsInventoryComponent* FixedSlotsInventoryComponent;

private:
	TArray<TObjectPtr<USlotWidget>> SlotWidgets;
};
