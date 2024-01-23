// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetBase/ItemWidget.h"
#include "WidgetBase/ItemDragDropOperation.h"
#include "Components/Image.h"
#include "Item/ItemObject.h"
#include "Item/ItemPropertyFragment.h"
#include "PropertyFragment/PropertyFragment_PropBaseInfo.h"

UDragDropOperation* UItemWidget::DragItem(TSubclassOf<UItemWidget> DragDropWidgetClass)
{
	UItemWidget* DefaultDragVisual = CreateWidget<UItemWidget>(this, DragDropWidgetClass);
	DefaultDragVisual->Item = Item;

	UItemDragDropOperation* DragDropOperation = NewObject<UItemDragDropOperation>(GetTransientPackage(), UItemDragDropOperation::StaticClass());
	DragDropOperation->DefaultDragVisual = DefaultDragVisual;
	DragDropOperation->Pivot = EDragPivot::CenterCenter;
	DragDropOperation->Item = Item;

	return DragDropOperation;
}
