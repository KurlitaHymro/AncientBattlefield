// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	SlotSet.Init(this, 10);
}

FItemSlotHandle UInventoryComponent::AddItem(UItemObject* Item)
{
	if (Item != nullptr)
	{
		auto Handle = SlotSet.AddItem(Item);
		if (Handle.IsValid())
		{
			OnAddItem.Broadcast(Item);
		}
		return Handle;
	}
	return FItemSlotHandle();
}

bool UInventoryComponent::RemoveItem(FItemSlotHandle SlotHandle)
{
	UItemObject* Item = SlotSet.FindItem(SlotHandle);

	if (Item != nullptr)
	{
		OnRemoveItem.Broadcast(Item);
	}

	return SlotSet.RemoveItem(SlotHandle);
}
