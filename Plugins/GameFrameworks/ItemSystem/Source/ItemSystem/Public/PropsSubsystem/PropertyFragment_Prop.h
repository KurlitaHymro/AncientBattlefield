// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemPropertyFragment.h"
#include "PropertyFragment_Prop.generated.h"

USTRUCT(BlueprintType, meta = (DisplayName = "Prop"))
struct FPropertyFragmentProp : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText BasicDescription;
};

/**
 * Most items have the properties of props, basic information includes names and icons.
 */
UCLASS()
class ITEMSYSTEM_API UPropertyFragment_Prop : public UItemPropertyFragment
{
	GENERATED_BODY()

public:
	virtual void Setup(const FName Template = "") override;

	virtual void InitFromRegistry(FName Template) override;

	virtual bool IsKeyMatch(FName Key) override;

	virtual FGameplayTag GetPropertyTag() override;

	virtual FName GetRegistryTypeName() override;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FName& GetName() { return PropertyFragment.Name; };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UTexture2D* GetIcon() { return PropertyFragment.Icon; };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FString& GetDescription() { return Description; };

public:
	UPROPERTY(BlueprintReadOnly)
	FPropertyFragmentProp PropertyFragment;

	UPROPERTY(BlueprintReadWrite)
	FString Description;

public:
	static FName RegistryType;

};
