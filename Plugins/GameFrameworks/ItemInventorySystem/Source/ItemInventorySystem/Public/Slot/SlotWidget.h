// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class ITEMINVENTORYSYSTEM_API USlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void InitFromItem(class UItemObject* Item);

protected:
	UFUNCTION(BlueprintCallable)
	FName GetName() { return Name; };

	UFUNCTION(BlueprintCallable)
	UTexture* GetIcon() { return Icon; };

private:
	FName Name;

	UTexture* Icon;
};
