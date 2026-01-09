// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HitTraceComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUniqueHitDelegate, FHitResult, HitResult);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ANCIENTBATTLEFIELD_API UHitTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHitTraceComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(BlueprintAssignable)
	FUniqueHitDelegate UniqueHitDelegate;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interface)
	TArray<FName> Sockets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interface)
	TArray<FHitResult> HitResult;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TraceSettings, meta = (Tooltip = "In case you trace by channel."))
	TEnumAsByte<ETraceTypeQuery> TraceChannel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TraceSettings, meta = (Tooltip = "True means trace complex, false means trace simple collisions."))
	uint32 bTraceComplex : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TraceSettings, meta = (Tooltip = "Actors to ignore (array)."))
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TraceSettings, meta = (Tooltip = "How you want to debug the trace if at all."))
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TraceSettings, meta = (Tooltip = "Color when tracing but not hitting."))
	FLinearColor TraceColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TraceSettings, meta = (Tooltip = "Color when hitting."))
	FLinearColor HitColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TraceSettings, meta = (Tooltip = "Custom duration for draw time."))
	float DrawTime;

public:
	UFUNCTION(BlueprintCallable)
	void Setup(UPrimitiveComponent* Reference, bool bSearchSocketsFromMesh = true);

	UFUNCTION(BlueprintCallable)
	void Teardown();

	UFUNCTION(BlueprintCallable)
	void SetSockets(const TArray<FName>& SetSockets);

	UFUNCTION(BlueprintCallable)
	void EnableTrace();

	UFUNCTION(BlueprintCallable)
	void DisableTrace();

private:
	UPROPERTY()
	class UPrimitiveComponent* ReferenceMesh;

	UPROPERTY()
	AActor* Owner;

	UPROPERTY()
	uint32 bTrace : 1;

	UPROPERTY()
	TMap<FName, FVector> SocketsLastLocations;

protected:
	UFUNCTION()
	void TickTrace();

	UFUNCTION()
	void UpdateSocketLocation();
};
