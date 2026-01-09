// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class ITEMSYSTEM_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InstanceEditable = true, ExposeOnSpawn = true))
	class UInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InstanceEditable = true, ExposeOnSpawn = true))
	TSubclassOf<class UItemWidget> ItemWidgetType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InstanceEditable = true, ExposeOnSpawn = true))
	FVector2D SizeOverride;

protected:
	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;

	UFUNCTION()
	virtual void OnAddItem(class UItemObject* Item, int32 SpecialValue);

	UFUNCTION()
	virtual void OnRemoveItem(class UItemObject* Item, int32 SpecialValue);

	UFUNCTION(BlueprintCallable, Category = "Drag and Drop")
	bool DropItem(UDragDropOperation* DragDropOperation);

	UFUNCTION(BlueprintImplementableEvent)
	UPanelWidget* GetSpecificPanel();

	UFUNCTION(BlueprintImplementableEvent)
	USizeBox* GetSpecificSizeBox();

protected:
	class UPanelWidget* Panel;

	FDelegateHandle OnAddItemDelegate;

	FDelegateHandle OnRemoveItemDelegate;

private:
	TMap<TObjectKey<UItemObject>, TObjectPtr<UItemWidget>> Widgets;
};
