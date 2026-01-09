// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemWidget.generated.h"

DECLARE_DELEGATE(F);

/**
 * 
 */
UCLASS()
class ITEMSYSTEM_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InstanceEditable = true, ExposeOnSpawn = true))
	class UItemObject* Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InstanceEditable = true, ExposeOnSpawn = true))
	FVector2D SizeOverride;

protected:
	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Drag and Drop")
	UDragDropOperation* DragItem(TSubclassOf<UItemWidget> VisualWidgetClass);

	UFUNCTION(BlueprintCallable, Category = "Drag and Drop")
	bool DropItem(UDragDropOperation* DragDropOperation);

	UFUNCTION(BlueprintCallable, Category = "Operation")
	void ConstructItemOperations(TSubclassOf<UUserWidget> OperationWidgetClass);

	UFUNCTION(BlueprintImplementableEvent)
	USizeBox* GetSpecificSizeBox();
};