// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemPropertyFragment.h"

UItemObject* UItemPropertyFragment::GetOwner() const
{
	GetOuter();

	return OwnerItemObject;
}

void UItemPropertyFragment::Instantiate(UItemObject* Owner)
{
	UItemObject* OwnerItem = Owner;

}

void UItemPropertyFragment::InitFromMetaDataTable(const UDataTable* DataTable, FString PrefabName)
{
	
}