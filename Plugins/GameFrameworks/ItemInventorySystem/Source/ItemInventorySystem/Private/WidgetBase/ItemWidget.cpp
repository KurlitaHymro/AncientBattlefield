// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetBase/ItemWidget.h"
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
	if (Item)
	if (!Icon)
	{
		UPropertyFragment_PropBaseInfo* BaseInfo = Item->FindPropertyFragment<UPropertyFragment_PropBaseInfo>();
		if (BaseInfo != nullptr)
		{
			Icon = BaseInfo->PropertyFragment.Icon;
		}
	}
	return Icon;
}
