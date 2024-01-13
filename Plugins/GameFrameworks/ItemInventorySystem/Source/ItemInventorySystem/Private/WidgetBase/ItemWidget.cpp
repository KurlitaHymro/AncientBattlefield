// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetBase/ItemWidget.h"
#include "WidgetBase/ItemDragDropOperation.h"
#include "Components/Image.h"
#include "Item/ItemObject.h"
#include "Item/ItemPropertyFragment.h"
#include "PropertyFragment/PropertyFragment_PropBaseInfo.h"

void UItemWidget::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt(); // 蓝图预构造、构造方法的调用时机。

	if (Item)
	{
		SetImageWidgetByIcon();
	}
}

FName UItemWidget::GetPropName()
{
	ensure(Item);
	FName PropName;
	UPropertyFragment_PropBaseInfo* BaseInfo = Item->FindPropertyFragment<UPropertyFragment_PropBaseInfo>();
	if (BaseInfo != nullptr)
	{
		PropName = BaseInfo->PropertyFragment.Name;
	}
	else
	{
		PropName = "Miss_Prop_Name";
	}
	return PropName;
}

UTexture2D* UItemWidget::GetPropIcon()
{
	ensure(Item);
	UTexture2D* PropIcon = nullptr;
	UPropertyFragment_PropBaseInfo* BaseInfo = Item->FindPropertyFragment<UPropertyFragment_PropBaseInfo>();
	if (BaseInfo != nullptr)
	{
		PropIcon = BaseInfo->PropertyFragment.Icon;
	}
	return PropIcon;
}

void UItemWidget::SetImageWidgetByIcon_Implementation()
{
	if (auto Image = GetImageWidget())
	{
		Image->SetBrushFromTexture(GetPropIcon());
	}
}

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
