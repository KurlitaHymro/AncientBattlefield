// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySubsystem/Widgets/InventoryWidget.h"
#include "InventorySubsystem/Widgets/ItemWidget.h"
#include "InventorySubsystem/Widgets/ItemDragDropOperation.h"
#include "InventorySubsystem/Components/InventoryComponent.h"
#include "InventorySubsystem/PropertyFragment_Inventory.h"
#include "Item/ItemObject.h"
#include "Components/PanelWidget.h"
#include "Components/SizeBox.h"

void UInventoryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (USizeBox* SizeBox = GetSpecificSizeBox())
	{
		SizeBox->SetWidthOverride(SizeOverride.X);
		SizeBox->SetHeightOverride(SizeOverride.Y);
	}
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (InventoryComponent && GetSpecificPanel())
	{
		Panel = GetSpecificPanel();
		for (UItemObject* Item : InventoryComponent->Items)
		{
			OnAddItem(Item, INDEX_NONE);
		}
		OnAddItemDelegate = InventoryComponent->InventoryAddItemDelegate.AddUObject(this, &ThisClass::OnAddItem);
		OnRemoveItemDelegate = InventoryComponent->InventoryRemoveItemDelegate.AddUObject(this, &ThisClass::OnRemoveItem);
	}
}

void UInventoryWidget::NativeDestruct()
{
	if (InventoryComponent)
	{
		InventoryComponent->InventoryAddItemDelegate.Remove(OnAddItemDelegate);
		InventoryComponent->InventoryRemoveItemDelegate.Remove(OnRemoveItemDelegate);
		InventoryComponent = nullptr;
	}

	Super::NativeDestruct();
}

void UInventoryWidget::OnAddItem(UItemObject* Item, int32 SpecialValue)
{
	auto ItemWidget = NewObject<UItemWidget>(this, ItemWidgetType);
	ItemWidget->Item = Item;
	Widgets.Add(Item, ItemWidget);
	Panel->AddChild(ItemWidget);
}

void UInventoryWidget::OnRemoveItem(UItemObject* Item, int32 SpecialValue)
{
	UItemWidget* ItemWidget = Widgets.FindAndRemoveChecked(Item);
	Panel->RemoveChild(ItemWidget);
}

bool UInventoryWidget::DropItem(UDragDropOperation* DragDropOperation)
{
	UItemDragDropOperation* ItemDragDropOperation = Cast<UItemDragDropOperation>(DragDropOperation);
	if (InventoryComponent && ItemDragDropOperation && ItemDragDropOperation->Item)
	{
		UItemObject* DragDropItem = ItemDragDropOperation->Item;
		if (auto InventoryProperty = DragDropItem->FindPropertyFragment<UPropertyFragment_Inventory>())
		{
			if (InventoryProperty->Inventory)
			{
				InventoryProperty->Inventory->Remove(DragDropItem); // todo optimize
			}
			InventoryComponent->Add(DragDropItem);
			return true;
		}
	}
	return false;
}
