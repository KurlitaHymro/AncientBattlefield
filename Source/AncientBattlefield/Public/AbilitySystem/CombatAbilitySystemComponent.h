// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/RegisteredAbilitySystemComponent.h"
#include "Engine/DataTable.h"
#include "CombatAbilitySystemComponent.generated.h"

USTRUCT(BlueprintType, meta = (DisplayName = "AbilitiyInfo"))
struct FAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftClassPtr<class UGameplayAbility> ClassType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture2D* Icon;
};

USTRUCT(BlueprintType, meta = (DisplayName = "DeriveAbilities"))
struct FBodyFormAbilities : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TMap<TSoftObjectPtr<class UInputAction>, FAbilityInfo> DeriveAbilities;
};

USTRUCT(BlueprintType)
struct FAdvanceInput
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTag AdvanceInputTag;

	UPROPERTY()
	int32 AbilityID;

	UPROPERTY()
	bool bIsPressed;

	UPROPERTY()
	TArray<TSoftClassPtr<UGameplayAbility>> BodyFormDeriveAbilities;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSwitchBodyFormDelegate, TSoftObjectPtr<class UInputAction>, InputAction, FAbilityInfo, AbilityInfo);

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class ANCIENTBATTLEFIELD_API UCombatAbilitySystemComponent : public URegisteredAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UCombatAbilitySystemComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Dagame")
	FGameplayTagContainer DamageDefault;

	UPROPERTY(BlueprintAssignable)
	FSwitchBodyFormDelegate SwitchBodyFormDelegate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BodyForm")
	FName DefaultBodyForm;

public:
	UFUNCTION(BlueprintCallable)
	void HandleHitEvent(FGameplayTag EventTag, AActor* Target);

	UFUNCTION(BlueprintCallable)
	void SwitchBodyForm(FName NewBodyForm);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsWaitingAdvanceInput() { return HasMatchingGameplayTag(AdvanceInput.AdvanceInputTag); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsAdvanceInputValid() { return AdvanceInput.AbilityID != 0; }

	UFUNCTION(BlueprintCallable)
	void CommitAdvanceInput(int32 AbilityID, bool bPress);

	UFUNCTION(BlueprintCallable)
	void ResetAdvanceInput();

	UFUNCTION(BlueprintCallable)
	void TriggerAdvanceInput();

	virtual void AbilityLocalInputPressed(int32 InputID) override;

	virtual void AbilityLocalInputReleased(int32 InputID) override;

private:
	void HandleDamageDefault(UAbilitySystemComponent* Target);

	FAdvanceInput AdvanceInput;
};
