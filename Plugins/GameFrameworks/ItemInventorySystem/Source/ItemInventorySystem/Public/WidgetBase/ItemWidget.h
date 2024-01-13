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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InstanceEditable = true, ExposeOnSpawn = true))
	class UItemObject* Item;

protected:
	virtual void OnWidgetRebuilt() override;

	UFUNCTION(BlueprintCallable)
	FName GetPropName();

	UFUNCTION(BlueprintCallable)
	UTexture2D* GetPropIcon();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	class UImage* GetImageWidget();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetImageWidgetByIcon();

	UFUNCTION(BlueprintCallable, Category = "Drag and Drop")
	UDragDropOperation* DragItem(TSubclassOf<UItemWidget> DragDropWidgetClass);

private:

};
