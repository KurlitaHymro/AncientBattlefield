// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemPropertyFragment.h"
#include "PropertyFragment_Equipment.generated.h"

/**
 * 
 */
UCLASS()
class ANCIENTBATTLEFIELD_API UPropertyFragment_Equipment : public UItemPropertyFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> EntityType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMeshComponent* ParentMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AttachSocket;

public:
	UFUNCTION(BlueprintCallable)
	void SpawnEntity();

	UFUNCTION(BlueprintCallable)
	void DestroyEntity();

	UFUNCTION(BlueprintCallable)
	void PutOn();

	UFUNCTION(BlueprintCallable)
	void TakeOff();

	UFUNCTION(BlueprintCallable)
	UMeshComponent* GetMesh();

private:
	AActor* Entity;

	UMeshComponent* EquipmentMesh;
};
