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
	UFUNCTION()
	void OnWeaponPutOn(class UPropertyFragment_Equipment* Equipment);

	UFUNCTION()
	void OnWeaponTakeOff(class UPropertyFragment_Equipment* Equipment);

	UFUNCTION(BlueprintCallable)
	class UHitTraceComponent* GetHitTraceComponent();

protected:
	UFUNCTION()
	void OnWeaponHit(FHitResult HitResult);

private:
	class UCombatAbilitySystemComponent* AbilitySystemComponent;

	class UHitTraceComponent* HitTraceComponent;

	class UMeshComponent* Mesh;
};
