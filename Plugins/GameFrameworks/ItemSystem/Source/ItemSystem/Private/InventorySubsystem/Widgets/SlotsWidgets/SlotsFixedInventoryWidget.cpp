// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySubsystem/Widgets/SlotsWidgets/SlotsFixedInventoryWidget.h"
#include "InventorySubsystem/Widgets/SlotsWidgets/SlotWidget.h"
#include "InventorySubsystem/Widgets/ItemWidget.h"
#include "InventorySubsystem/Components/FixedSlotsInventoryComponent.h"

void USlotsFixedInventoryWidget::NativeConstruct()
{
	Super::Super::NativeConstruct();

	FixedSlotsInventoryComponent = Cast<UFixedSlotsInventoryComponent>(InventoryComponent);
	SlotWidgets.Empty();
	SlotWidgets = GetSlotWidgetsArray();
	if (FixedSlotsInventoryComponent)
	{
		OnAddItemDelegate = FixedSlotsInventoryComponent->InventoryAddItemDelegate.AddUObject(this, &ThisClass::OnAddItem);
		OnRemoveItemDelegate = FixedSlotsInventoryComponent->InventoryRemoveItemDelegate.AddUObject(this, &ThisClass::OnRemoveItem);
	}
}

void USlotsFixedInventoryWidget::NativeDestruct()
{
	if (FixedSlotsInventoryComponent)
	{
		FixedSlotsInventoryComponent->InventoryAddItemDelegate.Remove(OnAddItemDelegate);
		FixedSlotsInventoryComponent->InventoryRemoveItemDelegate.Remove(OnRemoveItemDelegate);
		FixedSlotsInventoryComponent = nullptr;
	}

	Super::Super::NativeDestruct();
}

void USlotsFixedInventoryWidget::OnAddItem(UItemObject* Item, int32 Index)
{
	SlotWidgets.RangeCheck(Index);
	SlotWidgets[Index]->CreateItemWidget(Item, ItemWidgetType);
}

void USlotsFixedInventoryWidget::OnRemoveItem(UItemObject* Item, int32 Index)
{
	SlotWidgets.RangeCheck(Index);
	SlotWidgets[Index]->RemoveItemWidget();
}