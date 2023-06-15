// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemPropertyFragment.h"
#include "PropertyFragment_Equipment.generated.h"

USTRUCT(BlueprintType, meta = (DisplayName = "PropertyFragment_Equipment"))
struct ANCIENTBATTLEFIELD_API FPropertyFragmentEquipment : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName AttachSlot;
};

/**
 * 
 */
UCLASS()
class ANCIENTBATTLEFIELD_API UPropertyFragment_Equipment : public UItemPropertyFragment
{
	GENERATED_BODY()
	
protected:
	virtual void InitFromMetaDataTable(const class UDataTable* DataTable, FString PrefabName) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UMeshComponent* ParentMesh;

	UPROPERTY(BlueprintReadOnly)
	FPropertyFragmentEquipment PropertyFragment;
};
