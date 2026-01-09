// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySubsystem/PropertyFragment_Inventory.h"
#include "InventorySubsystem/Components/InventoryComponent.h"
#include "Item/ItemObject.h"
#include "ItemSystem.h"

FName UPropertyFragment_Inventory::RegistryType(TEXT("InventoryRegistry"));

void UPropertyFragment_Inventory::Setup(const FName Template)
{
	Super::Setup(Template);

	InventoryUpdateDelegate.AddDynamic(this, &ThisClass::OnInventoryUpdate);
}

void UPropertyFragment_Inventory::Teardown()
{
	InventoryUpdateDelegate.RemoveDynamic(this, &ThisClass::OnInventoryUpdate);

	Super::Teardown();
}

FGameplayTag UPropertyFragment_Inventory::GetPropertyTag()
{
	return ItemSystemGameplayTags::ItemSystem_Property_Inventory;
}

FName UPropertyFragment_Inventory::GetRegistryTypeName()
{
	return RegistryType;
}

FString UPropertyFragment_Inventory::GetDebugInformation() const
{
	FString Description = FString::Printf(TEXT("-[DebugInformation]-\n"));
	Description += FString::Printf(TEXT("%s\n"), *OwnerItem->GetName());
	Description += FString::Printf(TEXT("%s\n"), *this->GetName());
	Description += FString::Printf(TEXT("%s\n"), *Inventory->GetClass()->GetName());
	Description += FString::Printf(TEXT("%s\n"), *Inventory->GetName());
	Description += FString::Printf(TEXT("Special Value:[%04d]\n"), SpecialValue);
	return Description;
}

void UPropertyFragment_Inventory::RegisterInventoryOperation(int32 Priority, UItemPropertyFragment* InProperty, const FName& CallbackFunctionName, const FText& ShowText)
{
	FInventoryOperation& InventoryOperation = PriorityInventoryOperations.Add(Priority);
	InventoryOperation.ShowText = ShowText;
	InventoryOperation.CallbackDelegate.BindUFunction(InProperty, CallbackFunctionName);
	PriorityInventoryOperations.KeySort([](int32 A, int32 B) { return A < B; });
}

void UPropertyFragment_Inventory::UnregisterInventoryOperation(int32 Priority)
{
	FInventoryOperation* InventoryOperation = PriorityInventoryOperations.Find(Priority);
	if (InventoryOperation)
	{
		InventoryOperation->CallbackDelegate.Unbind();
		PriorityInventoryOperations.Remove(Priority);
	}
}

void UPropertyFragment_Inventory::OnInventoryUpdate(UInventoryComponent* InventoryComponent, int32 Special)
{
	Inventory = InventoryComponent;
	SpecialValue = Special;
}
