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
	virtual void Init(const FName Template) override;

	virtual void InitFromRegistry(FName Template) override;

	virtual FGameplayTag GetPropertyTag() override;

	virtual FName GetRegistryTypeName() override;

	virtual FGameplayTagContainer GetRequiredTags() override;

public:
	UFUNCTION(BlueprintCallable)
	class UHitTraceComponent* GetHitTraceComponent() { return HitTraceComponent; };

	UFUNCTION(BlueprintCallable)
	class UTrailingComponent* GetTrailingComponent() { return TrailingComponent; };

protected:
	UFUNCTION()
	void OnSpawnEntity(AActor* Entity);

	UFUNCTION()
	void OnWeaponHit(FHitResult HitResult);

	UFUNCTION()
	void OnWeaponPutOn();

	UFUNCTION()
	void OnWeaponTakeOff();

public:
	UPROPERTY(BlueprintReadOnly)
	FPropertyFragmentMeleeWeapon PropertyFragment;

private:
	class UCombatAbilitySystemComponent* AbilitySystemComponent;

	class UHitTraceComponent* HitTraceComponent;

	class UTrailingComponent* TrailingComponent;

public:
	static FGameplayTag PropertyTag;

	static FName RegistryType;

	static FGameplayTag HitEventTag;
};
