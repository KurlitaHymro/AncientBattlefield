// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemPropertyFragment.h"
#include "Item/ItemObject.h"

void UItemPropertyFragment::Init(UItemObject* Item, const FName PrefabName, const FName RegistryType, const UDataTable* DataTable)
{
	Item->ItemTagContainer.AddTag(FGameplayTag::RequestGameplayTag(GetPropertyTagName()));
	if (PrefabName.IsValid() && !PrefabName.IsNone())
	{
		if (DataTable)
		{
			InitFromDataTable(DataTable, PrefabName);
		}
		if (!RegistryType.IsNone())
		{
			InitFromRegistry(RegistryType, PrefabName);
		}
	}
	Item->AddPropertyFragment(this);
}

void UItemPropertyFragment::InitFromDataTable(const UDataTable* DataTable, FName PrefabName)
{

}

void UItemPropertyFragment::InitFromRegistry(const FName RegistryType, FName PrefabName)
{

}

FName UItemPropertyFragment::GetPropertyTagName()
{
	return FName();
}

FName UItemPropertyFragment::GetRegistryTypeName()
{
	return FName();
}