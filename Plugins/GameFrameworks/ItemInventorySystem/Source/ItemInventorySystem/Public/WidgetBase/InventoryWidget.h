// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class ITEMINVENTORYSYSTEM_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InstanceEditable = true, ExposeOnSpawn = true))
	class UInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InstanceEditable = true, ExposeOnSpawn = true))
	TSubclassOf<class USlotWidget> SlotWidgetType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InstanceEditable = true, ExposeOnSpawn = true))
	TSubclassOf<class UItemWidget> ItemWidgetType;

protected:
	virtual void OnWidgetRebuilt() override;

	UFUNCTION()
	void OnAddItem(class UItemObject* Item, int32 SlotID);

	UFUNCTION()
	void OnRemoveItem(class UItemObject* Item, int32 SlotID);

protected:
	/* 使用统一网格面板自动布局
	 * 根据PerRow自动生成并排布物品槽。
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (InstanceEditable = true, ExposeOnSpawn = true, AllowPrivateAccess = "true", ClampMin = "1", UIMin = "1"))
	int32 PerRow = 1;

	UFUNCTION(BlueprintImplementableEvent)
	class UUniformGridPanel* GetUniformGridPanel();

	UFUNCTION(BlueprintCallable)
	void UniformGridArrange(UUserWidget* Widget, int32 SlotsPerRow, int32 SlotID);

protected:
	/* 使用已有槽位布局
	 * 对现有布局的若干覆层生成物品槽
	 */
	UFUNCTION(BlueprintImplementableEvent)
	TArray<class UOverlay*> GetOverlayContainer();

private:
	TMap<int32, UItemWidget*> SlotsItemWidget;

};
