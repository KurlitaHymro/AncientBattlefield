// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemObject.h"
#include "Item/ItemPropertyFragment.h"
#include "DataRegistrySubsystem.h"
#include "ItemSystem.h"

void UItemObject::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(ItemTagContainer);
}

void UItemObject::AddPropertyFragment(UItemPropertyFragment* PropertyFragmentObject, FName TemplateName)
{
	if (PropertyFragmentObject->IsReadyToAdded(this))
	{
		PropertyFragmentObject->OwnerItem = this;
		PropertyFragmentObject->Setup(TemplateName);
		PropertyFragments.Add(PropertyFragmentObject);
	}
	else
	{
		UE_LOG(LogItemSystem, Warning, TEXT("AddPropertyFragment Lack of Dependency."));
		PropertyFragmentObject->BeginDestroy();
	}
}

void UItemObject::RemovePropertyFragment(UItemPropertyFragment* PropertyFragmentObject)
{
	auto Index = PropertyFragments.Find(PropertyFragmentObject);
	if (Index != INDEX_NONE)
	{
		PropertyFragmentObject->Teardown();
		PropertyFragments.RemoveAt(Index);
	}
	else
	{
		UE_LOG(LogItemSystem, Warning, TEXT("RemovePropertyFragment not Found."));
	}
}

UItemPropertyFragment* UItemObject::FindPropertyFragment(TSubclassOf<UItemPropertyFragment> PropertyFragmentType)
{
	if (PropertyFragmentType)
	{
		for (UItemPropertyFragment* Fragment : PropertyFragments)
		{
			if (Fragment->IsA(PropertyFragmentType))
			{
				return Fragment;
			}
		}
	}
	return nullptr;
}

FName UItemObject::RegistryType(TEXT("ItemRegistry"));
UItemObject* UItemObject::NewItemByRegistry(UObject* Outer, FName TemplateName)
{
	UItemObject* Item = NewObject<UItemObject>(Outer);
	auto ItemPrefabRegistry = UDataRegistrySubsystem::Get()->GetRegistryForType(RegistryType);
	if (ItemPrefabRegistry)
	{
		auto Template = ItemPrefabRegistry->GetCachedItem<FItemAbstract>(FDataRegistryId(RegistryType, TemplateName));
		if (Template != nullptr)
		{
			for (auto PropertyFragment : Template->PropertyFragments)
			{
				UClass* PropertyClass = PropertyFragment.PropertyClass.LoadSynchronous();
				UItemPropertyFragment* PropertyInstance = NewObject<UItemPropertyFragment>(Item, PropertyClass);
				Item->AddPropertyFragment(PropertyInstance, PropertyFragment.PropertyTemplate);
			}
			Item->ItemTagContainer.AppendTags(Template->ItemTagContainer);
		}
	}
	return Item;
}