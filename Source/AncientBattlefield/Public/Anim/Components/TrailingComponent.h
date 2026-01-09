// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TrailingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANCIENTBATTLEFIELD_API UTrailingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTrailingComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interface)
	FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interface)
	FVector LocationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interface)
	FRotator RotationOffset;

	// The niagara system template to use when spawning the niagara component
	UPROPERTY(EditAnywhere, Category = NiagaraSystem, meta = (ToolTip = "The niagara system to spawn for the notify state"))
	TObjectPtr<class UNiagaraSystem> Template;

public:
	UFUNCTION(BlueprintCallable)
	void RibbonTrailingSetup(UPrimitiveComponent* Reference, bool bSearchSocketsFromMesh = true, FName Start = "", FName End = "");

	UFUNCTION(BlueprintCallable)
	void Setup(UPrimitiveComponent* Reference);

	UFUNCTION(BlueprintCallable)
	void Teardown();

	UFUNCTION(BlueprintCallable)
	void EnableTrailing();

	UFUNCTION(BlueprintCallable)
	void DisableTrailing();

	// Return FXSystemComponent created from SpawnEffect
	UFUNCTION(BlueprintCallable)
	class UFXSystemComponent* GetSpawnedEffect();

private:
	UPROPERTY()
	class UPrimitiveComponent* ReferenceMesh;

	UPROPERTY()
	AActor* Owner;

private:
	class UFXSystemComponent* SpawnEffect(USceneComponent* MeshComp) const;

	FORCEINLINE FName GetSpawnedComponentTag()const { return GetFName(); }
};
