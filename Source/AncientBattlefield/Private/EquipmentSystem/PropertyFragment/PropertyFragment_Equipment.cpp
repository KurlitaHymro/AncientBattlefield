// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentSystem/PropertyFragment/PropertyFragment_Equipment.h"

void UPropertyFragment_Equipment::InitFromMetaDataTable(const UDataTable* DataTable, FString PrefabName)
{
	FPropertyFragmentEquipment* MetaData = DataTable->FindRow<FPropertyFragmentEquipment>(FName(*PrefabName), DataTable->GetName(), true);
	if (MetaData)
	{
		PropertyFragment = *MetaData;
	}

	Super::InitFromMetaDataTable(DataTable, PrefabName);
}