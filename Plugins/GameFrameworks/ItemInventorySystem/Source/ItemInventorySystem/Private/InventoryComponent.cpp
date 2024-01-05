// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Item/ItemObject.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	ItemObjectSlot.Empty();
}

void UInventoryComponent::PostLoad()
{
	Super::PostLoad();
	
	if (Size > 0)
	{
		ItemObjectSlot.Init(nullptr, Size);
	}
}

void UInventoryComponent::Setup(int32 SlotsNumber)
{
	Size = SlotsNumber;
	ItemObjectSlot.Init(nullptr, Size);
	InventorySetupDelegate.Broadcast(Size);
}

int32 UInventoryComponent::FindVacancy() const
{
	for (int32 SlotID = 0; SlotID < Size; SlotID++)
	{
		if (ItemObjectSlot[SlotID] == nullptr)
		{
			return SlotID;
		}
	}
	return Size;
}

bool UInventoryComponent::CanMoveTo(UItemObject* Item, int32 SlotID) const
{
	if (Item && 0 <= SlotID && SlotID < Size)
	{
		return true;
	}
	return false;
}

void UInventoryComponent::AddItem(UItemObject* Item, int32 SlotID)
{
	if (Item && 0 <= SlotID && SlotID < Size)
	{
		Item->BelongingInventory = this;
		Item->BelongingSlotID = SlotID;
		InventoryAddItemDelegate.Broadcast(Item, SlotID);
		ItemObjectSlot[SlotID] = Item;
	}
}

void UInventoryComponent::RemoveItem(UItemObject* Item)
{
	if (Item && Item->BelongingInventory == this && Item->BelongingSlotID < Size)
	{
		int32 SlotID = Item->BelongingSlotID;
		InventoryRemoveItemDelegate.Broadcast(Item, SlotID);
		Item->BelongingSlotID = Size;
		Item->BelongingInventory = nullptr;
		ItemObjectSlot[SlotID] = nullptr;
	}
}

void UInventoryComponent::RemoveItemFromSlot(int32 SlotID)
{
	ItemObjectSlot.RangeCheck(SlotID);
	RemoveItem(ItemObjectSlot[SlotID]);
}

UItemObject* UInventoryComponent::SwapItem(int32 SlotID, UItemObject* OtherItem)
{
	UItemObject* ThisItem = GetItem(SlotID);
	if (ThisItem && OtherItem && OtherItem->BelongingInventory)
	{
		UInventoryComponent* OtherInventory = OtherItem->BelongingInventory;
		int32 OtherSlot = OtherItem->BelongingSlotID;
		if (CanMoveTo(OtherItem, SlotID) && OtherInventory->CanMoveTo(ThisItem, OtherSlot))
		{
			this->RemoveItem(ThisItem);
			OtherInventory->RemoveItem(OtherItem);
			this->AddItem(OtherItem, SlotID);
			OtherInventory->AddItem(ThisItem, OtherSlot);
		}
	}
	return ThisItem;
}

UItemObject* UInventoryComponent::GetItem(int32 SlotID)
{
	ItemObjectSlot.RangeCheck(SlotID);
	return ItemObjectSlot[SlotID];
}

int32 UInventoryComponent::GetSize() const
{
	return Size;
}

FString UInventoryComponent::GetStaticDescription() const
{
	FString Description = FString::Printf(TEXT("%s::%s"), *GetOwner()->GetName(), *this->GetName());
	for (int32 i = 0; i < Size; i++)
	{
		if (ItemObjectSlot[i])
		{
			auto ItemInfo = ItemObjectSlot[i]->GetFName().ToString();
			Description += FString::Printf(TEXT("\n[%02d]{%s}"), i, *ItemInfo);
		}
	}
	return Description;
} 