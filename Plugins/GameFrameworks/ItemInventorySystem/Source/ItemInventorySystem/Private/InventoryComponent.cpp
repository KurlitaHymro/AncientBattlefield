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

	Slots.Empty();
}

void UInventoryComponent::PostLoad()
{
	Super::PostLoad();
	
	if (Size > 0)
	{
		Slots.Init(FInventorySlot(), Size);
	}
}

int32 UInventoryComponent::FindVacancy(UItemObject* Item) const
{
	for (int32 SlotID = 0; SlotID < Size; SlotID++)
	{
		if (Slots[SlotID].Item == nullptr && CanHold(Item, SlotID))
		{
			return SlotID;
		}
	}
	return Size;
}

bool UInventoryComponent::CanHold(UItemObject* Item, int32 SlotID) const
{
	if (Item && 0 <= SlotID && SlotID < Size)
	{
		FGameplayTagContainer ItemTags;
		Item->GetOwnedGameplayTags(ItemTags);
		if (ItemTags.HasAll(Slots[SlotID].ItemRequiredTags) && !ItemTags.HasAny(Slots[SlotID].ItemBlockedTags)) // 后续仍可追加更复杂的功能
		{
			return true;
		}
	}
	return false;
}

void UInventoryComponent::AddItem(UItemObject* Item, int32 SlotID)
{
	if (CanHold(Item, SlotID))
	{
		Slots[SlotID].Item = Item;
		InventoryAddItemDelegate.Broadcast(Item, SlotID);
		Item->InventoryUpdateDelegate.Broadcast(this, SlotID);
	}
}

void UInventoryComponent::RemoveItem(UItemObject* Item)
{
	if (Item && Item->BelongingInventory == this)
	{
		int32 SlotID = Item->BelongingSlotID;
		ensure(Item == Slots[SlotID].Item);
		Slots[SlotID].Item = nullptr;
		InventoryRemoveItemDelegate.Broadcast(Item, SlotID);
		Item->InventoryUpdateDelegate.Broadcast(nullptr, Size);
	}
}

void UInventoryComponent::RemoveItemFromSlot(int32 SlotID)
{
	Slots.RangeCheck(SlotID);
	RemoveItem(Slots[SlotID].Item);
}

void UInventoryComponent::AppendSlotTags(int32 SlotID, FGameplayTagContainer BlockedTags, FGameplayTagContainer RequiredTags)
{
	Slots.RangeCheck(SlotID);
	Slots[SlotID].ItemBlockedTags.AppendTags(BlockedTags);
	Slots[SlotID].ItemRequiredTags.AppendTags(RequiredTags);
}

UItemObject* UInventoryComponent::SwapItem(int32 SlotID, UItemObject* OtherItem)
{
	UItemObject* ThisItem = GetItem(SlotID);
	if (ThisItem && OtherItem && OtherItem->BelongingInventory)
	{
		UInventoryComponent* OtherInventory = OtherItem->BelongingInventory;
		int32 OtherSlot = OtherItem->BelongingSlotID;
		if (CanHold(OtherItem, SlotID) && OtherInventory->CanHold(ThisItem, OtherSlot))
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
	Slots.RangeCheck(SlotID);
	return Slots[SlotID].Item;
}

bool UInventoryComponent::CollectToUniversalSlots()
{
	/* 将移除所有物品，再按次序添加回万能槽位中。
	 * 万能槽位UI不足的部分不会显示，Size不足则会被销毁。
	**/
	TArray<TObjectPtr<UItemObject>> Items;
	for (auto Slot : Slots)
	{
		if (Slot.Item)
		{
			Items.Add(Slot.Item);
			RemoveItem(Slot.Item);
		}
	}
	auto Iterator = Items.CreateIterator();
	for (int32 i = 0; i < Size; i++)
	{
		if (Iterator && Slots[i].ItemBlockedTags.IsEmpty() && Slots[i].ItemRequiredTags.IsEmpty())
		{
			AddItem(*Iterator, i);
			Iterator++;
		}
	}
	if (Iterator)
	{
		return false;
	}
	else
	{
		return true;
	}
}

FString UInventoryComponent::GetStaticDescription() const
{
	FString Description = FString::Printf(TEXT("%s::%s"), *GetOwner()->GetName(), *this->GetName());
	for (int32 i = 0; i < Size; i++)
	{
		if (Slots[i].Item)
		{
			auto ItemInfo = Slots[i].Item->GetFName().ToString();
			Description += FString::Printf(TEXT("\n[%02d]{%s}"), i, *ItemInfo);
		}
	}
	return Description;
} 