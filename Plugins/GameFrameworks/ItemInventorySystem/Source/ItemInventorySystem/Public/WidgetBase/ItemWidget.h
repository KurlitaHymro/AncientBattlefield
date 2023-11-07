// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class ITEMINVENTORYSYSTEM_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UItemObject* Item;

protected:
	UFUNCTION(BlueprintCallable)
	FName GetName();

	UFUNCTION(BlueprintCallable)
	UTexture* GetIcon();

private:
	FName Name;

	UTexture* Icon;
};
