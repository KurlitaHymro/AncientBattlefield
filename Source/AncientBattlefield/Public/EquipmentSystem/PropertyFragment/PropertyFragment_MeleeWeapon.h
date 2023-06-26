// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemPropertyFragment.h"
#include "EquipmentSystem/EquipmentComponent.h"
#include "PropertyFragment_MeleeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ANCIENTBATTLEFIELD_API UPropertyFragment_MeleeWeapon : public UItemPropertyFragment
{
	GENERATED_BODY()

public:


protected:
	virtual void Instantiate(class UItemObject* Owner) override;

public:
	UFUNCTION()
	void OnWeaponPutOn();

	UFUNCTION()
	void OnWeaponTakeOff();

	UFUNCTION(BlueprintCallable)
	class UHitTraceComponent* GetHitTraceComponent();

private:
	class UHitTraceComponent* HitTraceComponent;

	class UMeshComponent* Mesh;
};
