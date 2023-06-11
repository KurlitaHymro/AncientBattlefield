// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemPropertyFragment.h"

void UItemPropertyFragment::OnItemInstantiate(UItemObject* Owner) const
{
	
}

void UItemPropertyFragment::InitFromMetaDataTable(const UDataTable* DataTable, FString PrefabName)
{
	OnRefresh();
}

void UItemPropertyFragment::OnRefresh() const
{

}
