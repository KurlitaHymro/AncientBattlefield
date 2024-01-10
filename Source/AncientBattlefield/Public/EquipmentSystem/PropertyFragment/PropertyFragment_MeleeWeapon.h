// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemPropertyFragment.h"
#include "PropertyFragment_MeleeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ANCIENTBATTLEFIELD_API UPropertyFragment_MeleeWeapon : public UItemPropertyFragment
{
	GENERATED_BODY()

public:
	virtual FName GetPropertyTagName() override;

public:
	UFUNCTION()
	void OnWeaponPutOn(class UAbilitySystemComponent* TargetASC);

	UFUNCTION()
	void OnWeaponTakeOff();

	UFUNCTION(BlueprintCallable)
	class UHitTraceComponent* GetHitTraceComponent() { return HitTraceComponent; };

	UFUNCTION(BlueprintCallable)
	class UTrailingComponent* GetTrailingComponent() { return TrailingComponent; };

protected:
	UFUNCTION()
	void OnWeaponHit(FHitResult HitResult);

private:
	class UCombatAbilitySystemComponent* AbilitySystemComponent;

	class UHitTraceComponent* HitTraceComponent;

	class UTrailingComponent* TrailingComponent;
};
