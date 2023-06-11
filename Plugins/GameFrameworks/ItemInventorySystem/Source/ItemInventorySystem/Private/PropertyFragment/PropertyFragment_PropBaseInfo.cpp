// Fill out your copyright notice in the Description page of Project Settings.


#include "PropertyFragment/PropertyFragment_PropBaseInfo.h"

void UPropertyFragment_PropBaseInfo::OnItemInstantiate(UItemObject* Owner) const
{

}

void UPropertyFragment_PropBaseInfo::InitFromMetaDataTable(const UDataTable* DataTable, FString PrefabName)
{
	FPropertyFragmentPropBaseInfo* MetaData = DataTable->FindRow<FPropertyFragmentPropBaseInfo>(FName(*PrefabName), DataTable->GetName(), true);
	if (MetaData)
	{
		PropertyFragment = *MetaData;
	}
	
	Super::InitFromMetaDataTable(DataTable, PrefabName);
}

void UPropertyFragment_PropBaseInfo::OnRefresh() const
{

}
