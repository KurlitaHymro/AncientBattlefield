// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ProfessionComponent.h"
#include "Engine/DataTable.h"
#include "CombatStanceComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSwitchCombatStanceDelegate, FName, Stance);

USTRUCT(BlueprintType, meta = (DisplayName = "StanceAbilities"))
struct FStanceAbilities : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TMap<TSoftObjectPtr<class UInputAction>, FAbilityInfo> StanceAbilities;
};

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class ANCIENTBATTLEFIELD_API UCombatStanceComponent : public UProfessionComponent
{
	GENERATED_BODY()
	
public:
	UCombatStanceComponent(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeComponent() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual int AddAbility(TSoftClassPtr<class UGameplayAbility> AbilityType, FName Name = "", UTexture* Icon = nullptr, class UInputAction* InputAction = nullptr) override;

	virtual void RemoveAbility(int32 AbilityID) override;

	virtual int32 QueryAbility(const class UInputAction* InputAction) const override;

	virtual FAbilityInfo GetAbilityInfo(int32 AbilityID) const override;

	virtual void SetupInputBinding() override;

	virtual void TeardownInputBinding() override;

	virtual void ReportInput(UInputAction* InputAction, FGameplayTag Tag, float Magnitude) override;

	UFUNCTION(BlueprintCallable, Category = "Abilities|Input")
	virtual void StashInput(UInputAction* InputAction, FGameplayTag Tag, float Magnitude);

	UFUNCTION(BlueprintCallable, Category = "Abilities|Input")
	virtual void PopInput();

	UFUNCTION()
	void RegisterStanceAbilities();

	UFUNCTION(BlueprintCallable)
	void SwitchCombatStance(FName NewStance);

	UFUNCTION(BlueprintCallable)
	void SettleStanceAbility();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Config")
	TSoftObjectPtr<class UDataTable> StanceAbilitiesTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Stance;

	UPROPERTY(BlueprintAssignable)
	FSwitchCombatStanceDelegate SwitchCombatStanceDelegate;

protected:
	static FName DefaultStance;

	int32 ActiveAbilityID;

	struct FInputStashInfo
	{
		FGameplayTag Tag;
		float Magnitude;
	};
	TArray<FInputStashInfo> StashInputList;
	class UInputAction* StashInputAction;

private:
	TMap<FName, TMap<UInputAction*, int32>> StanceAbilitiesMap;
};
