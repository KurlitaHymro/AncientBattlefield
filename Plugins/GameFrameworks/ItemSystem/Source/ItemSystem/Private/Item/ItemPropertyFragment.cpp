// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemPropertyFragment.h"
#include "Item/ItemObject.h"

void UItemPropertyFragment::Setup(const FName Template)
{
	ensure(OwnerItem);
	OwnerItem->ItemTagContainer.AddTag(GetPropertyTag());
	if (!Template.IsNone())
	{
		InitFromRegistry(Template);
	}
}

void UItemPropertyFragment::Teardown()
{
	ensure(OwnerItem);
	OwnerItem->ItemTagContainer.RemoveTag(GetPropertyTag());
}

void UItemPropertyFragment::InitFromRegistry(FName Template)
{

}

bool UItemPropertyFragment::IsReadyToAdded(UItemObject* Item)
{
	return Item->ItemTagContainer.HasAll(GetRequiredTags()) && !Item->ItemTagContainer.HasAny(GetBlockedTags());
}

bool UItemPropertyFragment::IsKeyMatch(FName Key)
{
	return false;
}

FGameplayTag UItemPropertyFragment::GetPropertyTag()
{
	return FGameplayTag();
}

FName UItemPropertyFragment::GetRegistryTypeName()
{
	return FName();
}

FString UItemPropertyFragment::GetPropertyDescription()
{
	return FString();
}

FGameplayTagContainer UItemPropertyFragment::GetRequiredTags()
{
	return FGameplayTagContainer();
}

FGameplayTagContainer UItemPropertyFragment::GetBlockedTags()
{
	return FGameplayTagContainer();
}