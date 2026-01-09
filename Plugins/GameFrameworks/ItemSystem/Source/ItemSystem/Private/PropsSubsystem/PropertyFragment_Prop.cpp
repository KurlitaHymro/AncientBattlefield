// Fill out your copyright notice in the Description page of Project Settings.


#include "PropsSubsystem/PropertyFragment_Prop.h"
#include "DataRegistrySubsystem.h"
#include "ItemSystem.h"

FName UPropertyFragment_Prop::RegistryType(TEXT("PropRegistry"));

void UPropertyFragment_Prop::Setup(const FName Template)
{
	Super::Setup(Template);

	Description = PropertyFragment.BasicDescription.ToString();
}

void UPropertyFragment_Prop::InitFromRegistry(FName Template)
{
	auto Registry = UDataRegistrySubsystem::Get()->GetRegistryForType(GetRegistryTypeName());
	if (Registry)
	{
		auto Prefab = Registry->GetCachedItem<FPropertyFragmentProp>(FDataRegistryId(GetRegistryTypeName(), Template));
		PropertyFragment = *Prefab;
	}
}

bool UPropertyFragment_Prop::IsKeyMatch(FName Key)
{
	return Key == GetName();
}

FGameplayTag UPropertyFragment_Prop::GetPropertyTag()
{
	return ItemSystemGameplayTags::ItemSystem_Property_Prop;
}

FName UPropertyFragment_Prop::GetRegistryTypeName()
{
	return RegistryType;
}