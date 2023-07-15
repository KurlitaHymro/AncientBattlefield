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
	// Sets default values for this actor's properties
	AEntityActor();

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Tooltip = "If this Actor will Create ItemObject. (Ensure any Item has only one PrimitiveActor)"))
	bool bPrimitiveActor;

	UPROPERTY(BlueprintReadOnly)
	class UItemObject* ItemObject;

	UPROPERTY()
	class UPropertyFragment_EntityLink* Link;

};
