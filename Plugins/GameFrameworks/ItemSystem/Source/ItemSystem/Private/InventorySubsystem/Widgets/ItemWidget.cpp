// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySubsystem/Widgets/ItemWidget.h"
#include "InventorySubsystem/Widgets/ItemDragDropOperation.h"
#include "InventorySubsystem/Components/FixedSlotsInventoryComponent.h"
#include "InventorySubsystem/PropertyFragment_Inventory.h"
#include "PropsSubsystem/PropertyFragment_Prop.h"
#include "Components/SizeBox.h"
#include "Item/ItemObject.h"

void UItemWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

    if (USizeBox* SizeBox = GetSpecificSizeBox())
    {
        SizeBox->SetWidthOverride(SizeOverride.X);
        SizeBox->SetHeightOverride(SizeOverride.Y);
    }
}

UDragDropOperation* UItemWidget::DragItem(TSubclassOf<UItemWidget> VisualWidgetClass)
{
	UItemWidget* VisualWidget = CreateWidget<UItemWidget>(this, VisualWidgetClass);
	VisualWidget->Item = Item;

	UItemDragDropOperation* ItemDragDropOperation = NewObject<UItemDragDropOperation>(GetTransientPackage(), UItemDragDropOperation::StaticClass());
	ItemDragDropOperation->DefaultDragVisual = VisualWidget;
	ItemDragDropOperation->Pivot = EDragPivot::CenterCenter;
	ItemDragDropOperation->Item = Item;

	return ItemDragDropOperation;
}

bool UItemWidget::DropItem(UDragDropOperation* DragDropOperation)
{
    UItemDragDropOperation* ItemDragDropOperation = Cast<UItemDragDropOperation>(DragDropOperation);
    if (ItemDragDropOperation && ItemDragDropOperation->Item)
    {
        UItemObject* DragItem = ItemDragDropOperation->Item;
        UItemObject* DropItem = Item;
        if (DragItem && DropItem && DragItem != DropItem)
        {
            UPropertyFragment_Inventory* DragItemInventoryProperty = DragItem->FindPropertyFragment<UPropertyFragment_Inventory>();
            UPropertyFragment_Inventory* DropItemInventoryProperty = DropItem->FindPropertyFragment<UPropertyFragment_Inventory>();
            ensure(DragItemInventoryProperty && DropItemInventoryProperty);
            
            bool CanSwap = true;
            UInventoryComponent* DragInventory = DragItemInventoryProperty->Inventory;
            UInventoryComponent* DropInventory = DropItemInventoryProperty->Inventory;
            ensure(DragInventory && DropInventory);

            UFixedSlotsInventoryComponent* DragFixedSlotsInventory = nullptr;
            UFixedSlotsInventoryComponent* DropFixedSlotsInventory = nullptr;
            if (DragInventory->IsA(UFixedSlotsInventoryComponent::StaticClass()))
            {
                DragFixedSlotsInventory = Cast<UFixedSlotsInventoryComponent>(DragInventory);
                CanSwap = CanSwap && DragFixedSlotsInventory->CanAccommodate(DragItemInventoryProperty->SpecialValue, DropItem);
            }
            if (DropInventory->IsA(UFixedSlotsInventoryComponent::StaticClass()))
            {
                DropFixedSlotsInventory = Cast<UFixedSlotsInventoryComponent>(DropInventory);
                CanSwap = CanSwap && DropFixedSlotsInventory->CanAccommodate(DropItemInventoryProperty->SpecialValue, DragItem);
            }

            if (CanSwap)
            {
                int32 DropIndex = INDEX_NONE;
                if (DropFixedSlotsInventory)
                {
                    DropIndex = DropItemInventoryProperty->SpecialValue;
                    DropFixedSlotsInventory->RemoveFrom(DropIndex);
                }
                else
                {
                    DropInventory->Remove(DropItem);
                }

                int32 DragIndex = INDEX_NONE;
                if (DragFixedSlotsInventory)
                {
                    DragIndex = DragItemInventoryProperty->SpecialValue;
                    DragFixedSlotsInventory->RemoveFrom(DragIndex);
                    DragFixedSlotsInventory->AddTo(DragIndex, DropItem);
                }
                else
                {
                    DragInventory->Remove(DragItem);
                    DragInventory->Add(DropItem);
                }

                if (DropFixedSlotsInventory)
                {
                    DropFixedSlotsInventory->AddTo(DropIndex, DragItem);
                }
                else
                {
                    DropInventory->Add(DragItem);
                }
            }
        }
    }
	return false;
}

void UItemWidget::ConstructItemOperations(TSubclassOf<UUserWidget> OperationWidgetClass)
{
    if (UPropertyFragment_Inventory* Inventory = Item->FindPropertyFragment<UPropertyFragment_Inventory>())
    {
        for (auto Operation : Inventory->PriorityInventoryOperations)
        {
            
        }
    }
}
