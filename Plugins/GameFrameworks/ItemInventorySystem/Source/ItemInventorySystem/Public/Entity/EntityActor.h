// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EntityActor.generated.h"

UCLASS()
class ITEMINVENTORYSYSTEM_API AEntityActor : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Tooltip = "If this Actor will Create ItemObject. (Ensure any Item has only one PrimitiveActor)"))
	bool bPrimitiveActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (InstanceEditable = true, ExposeOnSpawn = true))
	class UItemObject* ItemObject;

	UPROPERTY()
	class UPropertyFragment_EntityLink* Link;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CreateItem();
	virtual void CreateItem_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ResetItem();
	virtual void ResetItem_Implementation();
};