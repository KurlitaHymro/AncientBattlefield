// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class ITEMINVENTORYSYSTEM_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UItemObject* Item;
};
