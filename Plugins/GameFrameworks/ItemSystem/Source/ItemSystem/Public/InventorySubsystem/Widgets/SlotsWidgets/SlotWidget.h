// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class ITEMSYSTEM_API USlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InstanceEditable = true, ExposeOnSpawn = true))
	class UFixedSlotsInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InstanceEditable = true, ExposeOnSpawn = true))
	FVector2D SizeOverride;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InstanceEditable = true, ExposeOnSpawn = true))
	int32 SlotIndex;

public:
	UFUNCTION(BlueprintCallable)
	void CreateItemWidget(class UItemObject* Item, TSubclassOf<class UItemWidget> ItemWidgetType);

	UFUNCTION(BlueprintCallable)
	void RemoveItemWidget();

	UFUNCTION(BlueprintCallable, Category = "Drag and Drop")
	bool DropItem(UDragDropOperation* DragDropOperation);

protected:
	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	USizeBox* GetSpecificSizeBox();

	UFUNCTION(BlueprintImplementableEvent)
	UOverlay* GetSpecificOverlay();

private:
	TObjectPtr<class UItemWidget> ItemWidget;

	class UOverlay* Overlay;
};
