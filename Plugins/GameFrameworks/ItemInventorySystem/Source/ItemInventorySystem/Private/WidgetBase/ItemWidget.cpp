// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetBase/ItemWidget.h"
#include "WidgetBase/ItemDragDropOperation.h"
#include "Item/ItemObject.h"
#include "Item/ItemPropertyFragment.h"
#include "PropertyFragment/PropertyFragment_PropBaseInfo.h"

FName UItemWidget::GetName()
{
	if (Item)
	{
		if (!Name.IsValid())
		{
			UPropertyFragment_PropBaseInfo* BaseInfo = Item->FindPropertyFragment<UPropertyFragment_PropBaseInfo>();
			if (BaseInfo != nullptr)
			{
				Name = BaseInfo->PropertyFragment.Name;
			}
			else
			{
				Name = FName("Error_Item");
			}
		}
	}
	return Name;
}

UTexture* UItemWidget::GetIcon()
{
	if (Item && !Icon)
	{
		UPropertyFragment_PropBaseInfo* BaseInfo = Item->FindPropertyFragment<UPropertyFragment_PropBaseInfo>();
		if (BaseInfo != nullptr)
		{
			Icon = BaseInfo->PropertyFragment.Icon;
		}
	}
	return Icon;
}

UDragDropOperation* UItemWidget::DragItem(TSubclassOf<UItemWidget> DragDropWidgetClass)
{
	UItemWidget* DefaultDragVisual = CreateWidget<UItemWidget>(this, DragDropWidgetClass);
	DefaultDragVisual->Icon = Icon;

	UItemDragDropOperation* DragDropOperation = NewObject<UItemDragDropOperation>(GetTransientPackage(), UItemDragDropOperation::StaticClass());
	DragDropOperation->DefaultDragVisual = DefaultDragVisual;
	DragDropOperation->Pivot = EDragPivot::CenterCenter;
	DragDropOperation->Item = Item;

	return DragDropOperation;
}
