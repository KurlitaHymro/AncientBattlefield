// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySubsystem/Widgets/SlotsWidgets/SlotWidget.h"
#include "InventorySubsystem/Widgets/ItemDragDropOperation.h"
#include "InventorySubsystem/Widgets/ItemWidget.h"
#include "InventorySubsystem/Components/FixedSlotsInventoryComponent.h"
#include "InventorySubsystem/PropertyFragment_Inventory.h"
#include "Components/SizeBox.h"
#include "Components/Overlay.h"
#include "Item/ItemObject.h"

void USlotWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

    if (USizeBox* SizeBox = GetSpecificSizeBox())
    {
        SizeBox->SetWidthOverride(SizeOverride.X);
        SizeBox->SetHeightOverride(SizeOverride.Y);
    }

    Overlay = GetSpecificOverlay();
}

void USlotWidget::CreateItemWidget(class UItemObject* Item, TSubclassOf<class UItemWidget> ItemWidgetType)
{
    ItemWidget = NewObject<UItemWidget>(this, ItemWidgetType);
    ItemWidget->Item = Item;
    ItemWidget->SizeOverride = SizeOverride;
    if (Overlay)
    {
        Overlay->AddChild(ItemWidget);
    }
}

void USlotWidget::RemoveItemWidget()
{
    if (ItemWidget)
    {
        if (Overlay)
        {
            Overlay->RemoveChild(ItemWidget);
        }
        ItemWidget = nullptr;
    }
}

bool USlotWidget::DropItem(UDragDropOperation* DragDropOperation)
{
    UItemDragDropOperation* ItemDragDropOperation = Cast<UItemDragDropOperation>(DragDropOperation);
    if (InventoryComponent && ItemDragDropOperation && ItemDragDropOperation->Item)
    {
        UItemObject* DragDropItem = ItemDragDropOperation->Item;
        if (auto InventoryProperty = DragDropItem->FindPropertyFragment<UPropertyFragment_Inventory>())
        {
            if (InventoryComponent->CanAddTo(SlotIndex, DragDropItem) && InventoryProperty->Inventory)
            {
                InventoryProperty->Inventory->Remove(DragDropItem);
                InventoryComponent->AddTo(SlotIndex, DragDropItem);
                return true;
            }
        }
    }
    return false;
}
