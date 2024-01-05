// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetBase/SlotWidget.h"
#include "WidgetBase/ItemDragDropOperation.h"
#include "Item/ItemObject.h"
#include "InventoryComponent.h"

bool USlotWidget::DropItem(UItemDragDropOperation* ItemDragDropOperation)
{
    ensure(ItemDragDropOperation);

    auto Item = ItemDragDropOperation->Item;
    if (Item->BelongingInventory && InventoryComponent)
    {
        Item->BelongingInventory->RemoveItem(Item);
        InventoryComponent->AddItem(Item, SlotID);
        return true;
    }
    return false;
}
