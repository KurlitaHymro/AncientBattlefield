// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "GameFeatureAction_AddToWorld.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class GAMEFEATURESEXPANSIONS_API UGameFeatureAction_AddToWorld : public UGameFeatureAction
{
	GENERATED_BODY()
	
public:
	//~ Begin UGameFeatureAction interface
	virtual void OnGameFeatureActivating() override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
	//~ End UGameFeatureAction interface

protected:
	template<class ComponentType>
	ComponentType* FindOrAddComponentForActor(AActor* Actor, const TSoftClassPtr<AActor>& Entry)
	{
		return Cast<ComponentType>(FindOrAddComponentForActor(ComponentType::StaticClass(), Actor, Entry));
	}
	UActorComponent* FindOrAddComponentForActor(UClass* ComponentType, AActor* Actor, const TSoftClassPtr<AActor>& Entry);

	TArray<TSharedPtr<struct FComponentRequestHandle>> ComponentRequests;

private:
	void HandleGameInstanceStart(UGameInstance* GameInstance);
	virtual void AddToWorld(const FWorldContext& WorldContext) PURE_VIRTUAL(UGameFeatureAction_AddToWorld::AddToWorld, );

private:
	FDelegateHandle GameInstanceStartHandle;
};