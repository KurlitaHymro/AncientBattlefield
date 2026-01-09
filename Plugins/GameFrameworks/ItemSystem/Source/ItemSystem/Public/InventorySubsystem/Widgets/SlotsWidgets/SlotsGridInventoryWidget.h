// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySubsystem/Widgets/InventoryWidget.h"
#include "SlotsGridInventoryWidget.generated.h"

/**
 * 统一网格显示方案
 */
UCLASS()
class ITEMSYSTEM_API USlotsGridInventoryWidget : public UInventoryWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InstanceEditable = true, ExposeOnSpawn = true))
	TSubclassOf<class USlotWidget> SlotWidgetType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InstanceEditable = true, ExposeOnSpawn = true, ClampMin = "1.0", UIMin = "1.0"))
	int32 RowSlotsNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InstanceEditable = true, ExposeOnSpawn = true))
	FVector2D UnitOverride;

protected:
	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	virtual void OnAddItem(class UItemObject* Item, int32 SpecialValue) override;

	virtual void OnRemoveItem(class UItemObject* Item, int32 SpecialValue) override;

protected:
	class UFixedSlotsInventoryComponent* FixedSlotsInventoryComponent;

	class UUniformGridPanel* GridPanel;

private:
	TArray<TObjectPtr<USlotWidget>> SlotWidgets;
};
