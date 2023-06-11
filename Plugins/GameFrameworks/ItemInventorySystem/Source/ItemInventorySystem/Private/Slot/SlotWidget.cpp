// Fill out your copyright notice in the Description page of Project Settings.


#include "Slot/SlotWidget.h"
#include "Item/ItemObject.h"
#include "Item/ItemPropertyFragment.h"
#include "PropertyFragment/PropertyFragment_PropBaseInfo.h"

void USlotWidget::InitFromItem(UItemObject* Item)
{
	UPropertyFragment_PropBaseInfo* BaseInfo = Item->FindPropertyFragment<UPropertyFragment_PropBaseInfo>();
	if (BaseInfo != nullptr)
	{
		Name = BaseInfo->PropertyFragment.Name;
		Icon = BaseInfo->PropertyFragment.Icon;
	}
}
