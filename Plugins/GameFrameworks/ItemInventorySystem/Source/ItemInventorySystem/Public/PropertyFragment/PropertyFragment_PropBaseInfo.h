// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemPropertyFragment.h"
#include "PropertyFragment_PropBaseInfo.generated.h"

USTRUCT(BlueprintType, meta = (DisplayName = "PropBaseInfo"))
struct FPropertyFragmentPropBaseInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture2D* Icon;
};

/**
 * 
 */
UCLASS()
class ITEMINVENTORYSYSTEM_API UPropertyFragment_PropBaseInfo : public UItemPropertyFragment
{
	GENERATED_BODY()

public:
	virtual void InitFromRegistry(FName Template) override;

	virtual FGameplayTag GetPropertyTag() override;

	virtual FName GetRegistryTypeName() override;

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FName& GetName() { return PropertyFragment.Name; };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE UTexture2D* GetIcon() { return PropertyFragment.Icon; };

public:
	UPROPERTY(BlueprintReadOnly)
	FPropertyFragmentPropBaseInfo PropertyFragment;

public:
	static FGameplayTag PropertyTag;

	static FName RegistryType;

};