// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySubsystem/Widgets/SlotsWidgets/SlotsGridInventoryWidget.h"
#include "InventorySubsystem/Widgets/SlotsWidgets/SlotWidget.h"
#include "InventorySubsystem/Widgets/ItemWidget.h"
#include "InventorySubsystem/Components/FixedSlotsInventoryComponent.h"
#include "Components/UniformGridPanel.h"
#include "Components/SizeBox.h"

void USlotsGridInventoryWidget::NativePreConstruct()
{
	Super::Super::NativePreConstruct(); // Bind InventoryComponent in Blueprint.

	FixedSlotsInventoryComponent = Cast<UFixedSlotsInventoryComponent>(InventoryComponent);
	if (FixedSlotsInventoryComponent && SizeOverride.IsNearlyZero() && !UnitOverride.IsNearlyZero())
	{
		if (int32 Num = FixedSlotsInventoryComponent->SlotCount)
		{
			if (Num <= RowSlotsNum)
			{
				SizeOverride.X = Num * UnitOverride.X;
				SizeOverride.Y = UnitOverride.Y;
			}
			else
			{
				int32 Col = Num / RowSlotsNum;
				if (Num - Col * RowSlotsNum) { ++Col; }
				SizeOverride.X = RowSlotsNum * UnitOverride.X;
				SizeOverride.Y = Col * UnitOverride.Y;
			}
		}
	}

	if (USizeBox* SizeBox = GetSpecificSizeBox())
	{
		SizeBox->SetWidthOverride(SizeOverride.X);
		SizeBox->SetHeightOverride(SizeOverride.Y);
	}
}

void USlotsGridInventoryWidget::NativeConstruct()
{
	Super::Super::NativeConstruct();

	SlotWidgets.Empty();
	FixedSlotsInventoryComponent = Cast<UFixedSlotsInventoryComponent>(InventoryComponent);
	GridPanel = Cast<UUniformGridPanel>(GetSpecificPanel());
	if (FixedSlotsInventoryComponent && GridPanel)
	{
		SlotWidgets.Reserve(FixedSlotsInventoryComponent->FixedSlots.Num());
		for (int32 Index = 0; Index < FixedSlotsInventoryComponent->FixedSlots.Num(); Index++)
		{
			USlotWidget* SlotWidget = NewObject<USlotWidget>(this, SlotWidgetType);
			SlotWidget->SizeOverride = UnitOverride;
			SlotWidget->InventoryComponent = FixedSlotsInventoryComponent;
			SlotWidget->SlotIndex = Index;
			SlotWidgets.Emplace(SlotWidget);

			int32 Row = Index / RowSlotsNum;
			int32 Col = Index - Row * RowSlotsNum;
			GridPanel->AddChildToUniformGrid(SlotWidget, Row, Col);

			if (UItemObject* Item = FixedSlotsInventoryComponent->FixedSlots[Index].ItemObject)
			{
				SlotWidget->CreateItemWidget(Item, ItemWidgetType);
			}
		}
		OnAddItemDelegate = FixedSlotsInventoryComponent->InventoryAddItemDelegate.AddUObject(this, &ThisClass::OnAddItem);
		OnRemoveItemDelegate = FixedSlotsInventoryComponent->InventoryRemoveItemDelegate.AddUObject(this, &ThisClass::OnRemoveItem);
	}
}

void USlotsGridInventoryWidget::NativeDestruct()
{
	if (FixedSlotsInventoryComponent)
	{
		FixedSlotsInventoryComponent->InventoryAddItemDelegate.Remove(OnAddItemDelegate);
		FixedSlotsInventoryComponent->InventoryRemoveItemDelegate.Remove(OnRemoveItemDelegate);
		FixedSlotsInventoryComponent = nullptr;
	}

	Super::Super::NativeDestruct();
}

void USlotsGridInventoryWidget::OnAddItem(UItemObject* Item, int32 Index)
{
	SlotWidgets.RangeCheck(Index);
	SlotWidgets[Index]->CreateItemWidget(Item, ItemWidgetType);
}

void USlotsGridInventoryWidget::OnRemoveItem(UItemObject* Item, int32 Index)
{
	SlotWidgets.RangeCheck(Index);
	SlotWidgets[Index]->RemoveItemWidget();
}