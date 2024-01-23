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
		
		/* UI的部署会对槽位施加规则，所以需要对之前存在的物品重排；重排的过程实质是移除和添加。
		 * 移除时，之前存在的物品本就没有UI，也就不须关联代理；添加时，其UI会被创建。
		**/
		InventoryComponent->InventoryAddItemDelegate.AddDynamic(this, &ThisClass::OnAddItem);
		InventoryComponent->CollectItems();
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
		else if (!(Subregio + 1) && Subregio->SubregioBegin + Subregio->SubregioSize > SlotID)
		{
			break;
		}
		else
		{
			++Subregio;
		}
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
		else if (!(Subregio + 1) && Subregio->SubregioBegin + Subregio->SubregioSize > SlotID)
		{
			break;
		}
		else
		{
			++Subregio;
		}
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
	}
	AccumulativeSize += Subregio.SubregioSize;
}
