// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetBase/InventoryWidget.h"
#include "WidgetBase/SlotWidget.h"
#include "WidgetBase/ItemWidget.h"
#include "InventoryComponent.h"
#include "Components/UniformGridPanel.h"
#include "Components/Overlay.h"


void UInventoryWidget::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt(); // 蓝图预构造、构造方法的调用时机。

	if (InventoryComponent)
	{
		if (GetUniformGridPanel() && PerRow > 0)
		{
			for (int32 i = 0; i < InventoryComponent->GetSize(); i++)
			{
				auto SlotWidget = NewObject<USlotWidget>(this, SlotWidgetType);
				SlotWidget->InventoryComponent = InventoryComponent;
				SlotWidget->SlotID = i;
				UniformGridArrange(SlotWidget, PerRow, i);
				if (auto Item = InventoryComponent->GetItem(i))
				{
					OnAddItem(Item, i);
				}
			}
		}
		else if (!GetOverlayContainer().IsEmpty() && GetOverlayContainer().Num() == InventoryComponent->GetSize())
		{
			for (int32 i = 0; i < InventoryComponent->GetSize(); i++)
			{ 
				auto SlotWidget = NewObject<USlotWidget>(this, SlotWidgetType);
				SlotWidget->InventoryComponent = InventoryComponent;
				SlotWidget->SlotID = i;
				GetOverlayContainer()[i]->AddChildToOverlay(SlotWidget);
				if (auto Item = InventoryComponent->GetItem(i))
				{
					OnAddItem(Item, i);
				}
			}
		}
		InventoryComponent->InventoryAddItemDelegate.AddDynamic(this, &ThisClass::OnAddItem);
		InventoryComponent->InventoryRemoveItemDelegate.AddDynamic(this, &ThisClass::OnRemoveItem);
	}
}

void UInventoryWidget::OnAddItem(UItemObject* Item, int32 SlotID)
{
	auto ItemWidget = NewObject<UItemWidget>(this, ItemWidgetType);
	ItemWidget->Item = Item;
	SlotsItemWidget.Add(SlotID, ItemWidget);
	if (GetUniformGridPanel() && PerRow > 0)
	{
		UniformGridArrange(ItemWidget, PerRow, SlotID);
	}
	else if (!GetOverlayContainer().IsEmpty())
	{
		GetOverlayContainer()[SlotID]->AddChildToOverlay(ItemWidget);
	}
}

void UInventoryWidget::OnRemoveItem(UItemObject* Item, int32 SlotID)
{
	auto ItemWidget = SlotsItemWidget.FindAndRemoveChecked(SlotID);
	if (GetUniformGridPanel() && ItemWidget)
	{
		GetUniformGridPanel()->RemoveChild(ItemWidget);
	}
	else if (!GetOverlayContainer().IsEmpty())
	{
		GetOverlayContainer()[SlotID]->RemoveChild(ItemWidget);
	}
}

void UInventoryWidget::UniformGridArrange(UUserWidget* Widget, int32 SlotsPerRow, int32 SlotID)
{
	int32 Row = SlotID / SlotsPerRow;
	int32 Column = SlotID - Row * SlotsPerRow;
	GetUniformGridPanel()->AddChildToUniformGrid(Widget, Row, Column);
}
