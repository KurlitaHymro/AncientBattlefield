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
		GetSubregioConfig();
		AccumulativeSize = 0;
		for (auto& Subregio : SubregioConfig)
		{
			LoadSubregio(Subregio);
		}
		InventoryComponent->InventoryAddItemDelegate.AddDynamic(this, &ThisClass::OnAddItem);
		InventoryComponent->InventoryRemoveItemDelegate.AddDynamic(this, &ThisClass::OnRemoveItem);
	}
}

void UInventoryWidget::OnAddItem(UItemObject* Item, int32 SlotID)
{
	auto Subregio = SubregioConfig.CreateIterator();
	while (Subregio)
	{
		if (Subregio->SubregioBegin > SlotID)
		{
			--Subregio;
			break;
		}
		++Subregio;
	}
	if (Subregio)
	{
		auto ItemWidget = NewObject<UItemWidget>(this, Subregio->ItemWidgetType);
		ItemWidget->Item = Item;
		ItemsWidget.Add(SlotID, ItemWidget);

		if (Subregio->Panel->IsA(UUniformGridPanel::StaticClass()))
		{
			UUniformGridPanel* UniformGridPanel = Cast<UUniformGridPanel>(Subregio->Panel);
			int32 LocolID = SlotID - Subregio->SubregioBegin;
			int32 Row = LocolID / Subregio->SubregioGridPerRow;
			int32 Column = LocolID - Row * Subregio->SubregioGridPerRow;
			UniformGridPanel->AddChildToUniformGrid(ItemWidget, Row, Column);
		}
		else if (0) // 垂直表
		{

		}
		else if (Subregio->Panel->IsA(UOverlay::StaticClass()) && Subregio->SubregioSize == 1)
		{
			UOverlay* Overlay = Cast<UOverlay>(Subregio->Panel);
			Overlay->AddChildToOverlay(ItemWidget);
		}
	}
}

void UInventoryWidget::OnRemoveItem(UItemObject* Item, int32 SlotID)
{
	auto ItemWidget = ItemsWidget.FindAndRemoveChecked(SlotID);
	auto Subregio = SubregioConfig.CreateIterator();
	while (Subregio)
	{
		if (Subregio->SubregioBegin > SlotID)
		{
			--Subregio;
			break;
		}
		++Subregio;
	}
	if (Subregio)
	{
		Subregio->Panel->RemoveChild(ItemWidget);
	}
}



void UInventoryWidget::LoadSubregio(FInventoryWidgetSubregioInfo& Subregio)
{
	Subregio.SubregioBegin = AccumulativeSize;
	if (Subregio.Panel->IsA(UUniformGridPanel::StaticClass()))
	{
		UUniformGridPanel* UniformGridPanel = Cast<UUniformGridPanel>(Subregio.Panel);
		for (int32 LocolID = 0; LocolID < Subregio.SubregioSize; LocolID++)
		{
			auto SlotWidget = NewObject<USlotWidget>(this, Subregio.SlotWidgetType);
			SlotWidget->InventoryComponent = InventoryComponent;
			SlotWidget->SlotID = LocolID + AccumulativeSize;
			int32 Row = LocolID / Subregio.SubregioGridPerRow;
			int32 Column = LocolID - Row * Subregio.SubregioGridPerRow;
			UniformGridPanel->AddChildToUniformGrid(SlotWidget, Row, Column);
			if (auto Item = InventoryComponent->GetItem(SlotWidget->SlotID))
			{
				OnAddItem(Item, SlotWidget->SlotID);
			}
		}
	}
	else if (0) // 垂直表
	{

	}
	else if (Subregio.Panel->IsA(UOverlay::StaticClass()) && Subregio.SubregioSize == 1)
	{
		UOverlay* Overlay = Cast<UOverlay>(Subregio.Panel);
		auto SlotWidget = NewObject<USlotWidget>(this, Subregio.SlotWidgetType);
		SlotWidget->InventoryComponent = InventoryComponent;
		SlotWidget->SlotID = AccumulativeSize;
		Overlay->AddChildToOverlay(SlotWidget);
		if (auto Item = InventoryComponent->GetItem(SlotWidget->SlotID))
		{
			OnAddItem(Item, SlotWidget->SlotID);
		}
	}
	AccumulativeSize += Subregio.SubregioSize;
}
