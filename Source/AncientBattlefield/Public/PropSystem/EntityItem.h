// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EntityItem.generated.h"


UCLASS()
class ANCIENTBATTLEFIELD_API AEntityItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEntityItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite)
	TArray<struct FPropertyFragmentInit> GrantPropertyFragments;

	UPROPERTY(BlueprintReadWrite)
	class UItemObject* ItemObject;
};
