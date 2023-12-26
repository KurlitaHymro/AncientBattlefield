// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeature/GameFeatureAction_AddItem.h"
#include "Components/GameFrameworkComponentManager.h"
#include "InventoryComponent.h"
#include "Item/ItemObject.h"
#include "DataRegistrySubsystem.h"

void UGameFeatureAction_AddItem::AddToWorld(const FWorldContext& WorldContext)
{
	UWorld* World = WorldContext.World();
	UGameInstance* GameInstance = WorldContext.OwningGameInstance;

	if ((GameInstance != nullptr) && (World != nullptr) && World->IsGameWorld())
	{
		if (UGameFrameworkComponentManager* ComponentMan = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
		{
			int32 EntryIndex = 0;
			for (const auto& Entry : EntriesList)
			{
				if (!Entry.ActorClass.IsNull())
				{
					UGameFrameworkComponentManager::FExtensionHandlerDelegate AddAbilitiesDelegate = UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(
						this, &ThisClass::HandleActorExtension, EntryIndex);
					TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle = ComponentMan->AddExtensionHandler(Entry.ActorClass, AddAbilitiesDelegate);

					ComponentRequests.Add(ExtensionRequestHandle);
					EntryIndex++;
				}
			}
		}
	}
}

void UGameFeatureAction_AddItem::Reset()
{
	while (!ActiveExtensions.IsEmpty())
	{
		auto ExtensionIt = ActiveExtensions.CreateIterator();
		RemoveActorItems(ExtensionIt->Key);
	}

	ComponentRequests.Empty();
}

void UGameFeatureAction_AddItem::HandleActorExtension(AActor* Actor, FName EventName, int32 EntryIndex)
{
	if (EntriesList.IsValidIndex(EntryIndex))
	{
		const auto& Entry = EntriesList[EntryIndex];
		if (EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved/* || EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved*/)
		{
			RemoveActorItems(Actor);
		}
		else if (/*EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded || */EventName == UGameFrameworkComponentManager::NAME_GameActorReady)
		{
			AddActorItems(Actor, Entry);
		}
	}
}

void UGameFeatureAction_AddItem::AddActorItems(AActor* Actor, const FInventoryItemsEntry& Entry)
{
	if (UInventoryComponent* InventoryComponent = FindOrAddComponentForActor<UInventoryComponent>(Actor, Entry.ActorClass))
	{
		// 如果一个Actor具有多个库存（比如Character同时有物品栏和装备栏），这里只往首个库存中添加物品，需保证兼容性最高的库存在首个。
		TArray<class UItemObject*> AddedExtensions;
		AddedExtensions.Reserve(Entry.GivedItems.Num());

		for (const auto& ItemName : Entry.GivedItems)
		{
			if (ItemName.IsValid() && !ItemName.IsNone())
			{
				UItemObject* ItemObject = UItemObject::NewItem(Actor, ItemName);
				InventoryComponent->AddItem(ItemObject, InventoryComponent->FindVacancy());
				AddedExtensions.Add(ItemObject);
			}
		}

		ActiveExtensions.Add(Actor, AddedExtensions);
	}
}

void UGameFeatureAction_AddItem::RemoveActorItems(AActor* Actor)
{
	if (TArray<class UItemObject*>* AddedExtensions = ActiveExtensions.Find(Actor))
	{
		if (UInventoryComponent* InventoryComponent = Actor->FindComponentByClass<UInventoryComponent>())
		{
			for (UItemObject* ItemObject : *AddedExtensions)
			{
				InventoryComponent->RemoveItem(ItemObject);
			}
		}
		ActiveExtensions.Remove(Actor);
	}
}
