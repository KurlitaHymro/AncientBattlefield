// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "GameplayTagsManager.h"
#include "ProfessionComponent.generated.h"

USTRUCT(BlueprintType, meta = (DisplayName = "AbilitiyInfo"))
struct FAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSoftClassPtr<class UGameplayAbility> AbilityType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture* Icon;
};

/**
 * 
 */
UCLASS()
class COMBATFRAMEWORK_API UProfessionComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	UProfessionComponent(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeComponent() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual int AddAbility(TSoftClassPtr<class UGameplayAbility> AbilityType, FName Name = "", UTexture* Icon = nullptr, class UInputAction* InputAction = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void RemoveAbility(int32 AbilityID);

	UFUNCTION(BlueprintCallable, Category = "Abilities|Input")
	virtual int32 QueryAbility(const class UInputAction* InputAction) const;

	UFUNCTION(BlueprintCallable, Category = "Abilities|Input")
	virtual FAbilityInfo GetAbilityInfo(int32 AbilityID) const;

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void SetupInputBinding();

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void TeardownInputBinding();

	UFUNCTION(BlueprintCallable, Category = "Abilities|Input")
	virtual void ReportInput(UInputAction* InputAction, FGameplayTag Tag, float Magnitude);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsActiveAbilitiesInput;

protected:
	class URegisteredAbilitySystemComponent* RegisteredAbilitySystemComponent;

	class UAbilitiesInputComponent* AbilitiesInputComponent;

	TMap<class UInputAction*, int32> MappedAbilities;

	TMap<int32, FAbilityInfo> AbilitiesInfo;
};
