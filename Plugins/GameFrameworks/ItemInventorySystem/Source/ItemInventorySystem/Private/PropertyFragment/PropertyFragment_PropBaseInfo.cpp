// Fill out your copyright notice in the Description page of Project Settings.


#include "PropertyFragment/PropertyFragment_PropBaseInfo.h"
#include "Engine/DataTable.h"

void UPropertyFragment_PropBaseInfo::OnItemInstantiate() const
{

}

void UPropertyFragment_PropBaseInfo::InitFromMetaDataTable(const UDataTable* DataTable, FString PrefabName)
{
	// TODO¸¸Ààº¯ÊýÄ£°å
	FPropertyFragmentPropBaseInfo* MetaData = DataTable->FindRow<FPropertyFragmentPropBaseInfo>(FName(*PrefabName), DataTable->GetName(), true);
}
