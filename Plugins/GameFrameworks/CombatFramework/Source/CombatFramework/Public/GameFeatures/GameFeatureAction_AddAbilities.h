// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction_AddToWorld.h"
#include "GameFeatureAction_AddAbilities.generated.h"

USTRUCT(BlueprintType)
struct FCombatAbilityMapping
{
	GENERATED_BODY()

	// Type of ability to grant
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<class UGameplayAbility> AbilityType;

	// Ability name, used for display purposes, if any (can be left unset)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	// Icon to use for the ability, if any (can be left unset)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture* Icon;

	// Input action to bind the ability to, if any (can be left unset)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UInputAction> InputAction;
};

USTRUCT(BlueprintType)
struct FCombatAttributesMapping
{
	GENERATED_BODY()

	// Ability set to grant
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<class UAttributeSet> AttributeSetType;

	// Data table referent to initialize the attributes with, if any (can be left unset)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<class UDataTable> InitializationData;
};

USTRUCT()
struct FCombatAbilitiesEntry
{
	GENERATED_BODY()

	// The base actor class to add to
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TSoftClassPtr<AActor> ActorClass;

	// List of abilities to grant to actors of the specified class
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<FCombatAbilityMapping> GrantedAbilities;

	// List of attribute sets to grant to actors of the specified class 
	UPROPERTY(EditAnywhere, Category = "Attributes")
	TArray<FCombatAttributesMapping> GrantedAttributes;
};

//////////////////////////////////////////////////////////////////////
// UGameFeatureAction_AddAbilities

/**
 * GameFeatureAction responsible for granting abilities (and attributes) to actors of a specified type.
 */
UCLASS(MinimalAPI, meta = (DisplayName = "Add Abilities And Attributes"))
class UGameFeatureAction_AddAbilities final : public UGameFeatureAction_AddToWorld
{
	GENERATED_BODY()

public:
	//~ Begin UGameFeatureAction interface
	virtual void OnGameFeatureActivating() override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
#if WITH_EDITORONLY_DATA
	virtual void AddAdditionalAssetBundleData(FAssetBundleData& AssetBundleData) override;
#endif
	//~ End UGameFeatureAction interface

	//~ Begin UObject interface
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif
	//~ End UObject interface

	/**  */
	UPROPERTY(EditAnywhere, Category = "Abilities", meta = (TitleProperty = "ActorClass", ShowOnlyInnerProperties))
	TArray<FCombatAbilitiesEntry> EntriesList;

private:
	//~ Begin UGameFeatureAction_WorldActionBase interface
	virtual void AddToWorld(const FWorldContext& WorldContext) override;
	//~ End UGameFeatureAction_WorldActionBase interface

	void Reset();
	void HandleActorExtension(AActor* Actor, FName EventName, int32 EntryIndex);
	void AddActorAbilities(AActor* Actor, const FCombatAbilitiesEntry& Entry);
	void RemoveActorAbilities(AActor* Actor);

private:
	struct FActorExtensions
	{
		class URegisteredAbilitySystemComponent* AbilitySystemComponent;
		class UProfessionComponent* ProfessionComponent;

		TArray<int32> Abilities;
		TArray<class UAttributeSet*> Attributes;
	};
	TMap<AActor*, FActorExtensions> ActiveExtensions;
};