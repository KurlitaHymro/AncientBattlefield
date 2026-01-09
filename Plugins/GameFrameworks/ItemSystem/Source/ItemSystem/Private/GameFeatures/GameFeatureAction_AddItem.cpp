// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFeatures/GameFeatureAction_AddItem.h"
#include "Components/GameFrameworkComponentManager.h"
#include "InventorySubsystem/Components/InventoryComponent.h"
#include "InventorySubsystem/InventorySystemInterface.h"
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
					UGameFrameworkComponentManager::FExtensionHandlerDelegate AddAbilitiesDelegate =
						UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandleActorExtension, EntryIndex);
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
		auto ExtensionIterator = ActiveExtensions.CreateIterator();
		RemoveItems(ExtensionIterator->Key);
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
			RemoveItems(GetActorInventoryComponent(Actor));
		}
		else if (/*EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded || */EventName == UGameFrameworkComponentManager::NAME_GameActorReady)
		{
			AddItems(Actor, Entry);
		}
	}
}

void UGameFeatureAction_AddItem::AddItems(AActor* Actor, const FInventoryItemsEntry& Entry)
{
	if (UInventoryComponent* InventoryComponent = GetActorInventoryComponent(Actor))
	{
		TArray<class UItemObject*> AddedExtensions;
		AddedExtensions.Reserve(Entry.GivedItems.Num());
	
		for (const auto& ItemName : Entry.GivedItems)
		{
			if (ItemName.IsValid() && !ItemName.IsNone())
			{
				UItemObject* ItemObject = UItemObject::NewItemByRegistry(Actor, ItemName);
				InventoryComponent->Add(ItemObject);
				AddedExtensions.Add(ItemObject);
			}
		}
		ActiveExtensions.Add(InventoryComponent, AddedExtensions);
	}
}

void UGameFeatureAction_AddItem::RemoveItems(UInventoryComponent* InventoryComponent)
{
	if (TArray<class UItemObject*>* AddedExtensions = ActiveExtensions.Find(InventoryComponent))
	{
		for (UItemObject* ItemObject : *AddedExtensions)
		{
			InventoryComponent->Remove(ItemObject);
		}
		ActiveExtensions.Remove(InventoryComponent);
	}
}

UInventoryComponent* UGameFeatureAction_AddItem::GetActorInventoryComponent(AActor* Actor)
{
	UInventoryComponent* InventoryComponent;
	IInventorySystemInterface* InventoryInterface = Cast<IInventorySystemInterface>(Actor);
	if (InventoryInterface)
	{
		InventoryComponent = InventoryInterface->GetInventoryComponent();
	}
	else
	{
		InventoryComponent = FindOrAddComponentForActor<UInventoryComponent>(Actor, Actor->GetClass());
	}
	ensureAlways(InventoryComponent);
	return InventoryComponent;
}
