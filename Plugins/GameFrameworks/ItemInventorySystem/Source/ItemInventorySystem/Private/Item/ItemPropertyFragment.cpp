// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemPropertyFragment.h"
#include "Item/ItemObject.h"

void UItemPropertyFragment::Init(const FName Template)
{
	ensure(Owner);
	Owner->ItemTagContainer.AddTag(GetPropertyTag());
	if (!Template.IsNone())
	{
		InitFromRegistry(Template);
	}
}

void UItemPropertyFragment::InitFromRegistry(FName Template)
{

}

bool UItemPropertyFragment::IsDependencyReady(UItemObject* Item)
{
	return Item->ItemTagContainer.HasAll(GetRequiredTags()) && !Item->ItemTagContainer.HasAny(GetBlockedTags());
}

FGameplayTag UItemPropertyFragment::GetPropertyTag()
{
	return FGameplayTag();
}

FName UItemPropertyFragment::GetRegistryTypeName()
{
	return FName();
}

FGameplayTagContainer UItemPropertyFragment::GetRequiredTags()
{
	return FGameplayTagContainer();
}

FGameplayTagContainer UItemPropertyFragment::GetBlockedTags()
{
	return FGameplayTagContainer();
}