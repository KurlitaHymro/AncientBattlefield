// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemPropertyFragment.h"
#include "PropertyFragment_MeleeWeapon.generated.h"

USTRUCT(BlueprintType, meta = (DisplayName = "MeleeWeapon"))
struct FPropertyFragmentMeleeWeapon : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture2D* DisplayIcon;
};


/**
 * 
 */
UCLASS()
class ANCIENTBATTLEFIELD_API UPropertyFragment_MeleeWeapon : public UItemPropertyFragment
{
	GENERATED_BODY()

public:
	virtual void InitFromDataTable(const class UDataTable* DataTable, FName PrefabName) override;

	virtual void InitFromRegistry(const FName RegistryType, FName PrefabName) override;

	virtual FName GetPropertyTagName() override;

	virtual FName GetRegistryTypeName() override;

public:
	UFUNCTION()
	void OnWeaponPutOn();

	UFUNCTION()
	void OnWeaponTakeOff();

	UFUNCTION(BlueprintCallable)
	class UHitTraceComponent* GetHitTraceComponent() { return HitTraceComponent; };

	UFUNCTION(BlueprintCallable)
	class UTrailingComponent* GetTrailingComponent() { return TrailingComponent; };

protected:
	UFUNCTION()
	void OnWeaponHit(FHitResult HitResult);

public:
	UPROPERTY(BlueprintReadOnly)
	FPropertyFragmentMeleeWeapon PropertyFragment;

private:
	class UCombatAbilitySystemComponent* AbilitySystemComponent;

	class UHitTraceComponent* HitTraceComponent;

	class UTrailingComponent* TrailingComponent;
};
