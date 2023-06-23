// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/Components/HitTraceComponent.h"

// Sets default values for this component's properties
UHitTraceComponent::UHitTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Owner = nullptr;
	bTrace = false;
	SocketsLastLocations.Empty();

	TraceChannel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Pawn);
	bTraceComplex = false;
	ActorsToIgnore.Empty();
	DrawDebugType = EDrawDebugTrace::None;
	bIgnoreSelf = true;
	TraceColor = FLinearColor::Yellow;
	HitColor = FLinearColor::Red;
	DrawTime = 0.2f;
}


// Called when the game starts
void UHitTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHitTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bTrace)
	{
		TickTrace();
		UpdateSocketLocation();
	}
}

void UHitTraceComponent::Setup(UPrimitiveComponent* Reference)
{
	ReferenceMesh = Reference;
	Owner = GetOwner();
	ActorsToIgnore.Add(Owner);
	Sockets = ReferenceMesh->GetAllSocketNames();
}

void UHitTraceComponent::EnableTrace()
{
	if (ReferenceMesh == nullptr)
	{
		return;
	}
	bTrace = true;
	HitResult.Empty();
	SocketsLastLocations.Empty();
}

void UHitTraceComponent::DisableTrace()
{
	bTrace = false;
}

void UHitTraceComponent::TickTrace()
{
	for (auto Socket : Sockets)
	{
		auto LastLocation = SocketsLastLocations.Find(Socket);
		if (LastLocation == nullptr)
		{
			return;
		}
		const FVector Start = *LastLocation;
		const FVector End = ReferenceMesh->GetSocketLocation(Socket);

		TArray<FHitResult> OutHits;
		UKismetSystemLibrary::LineTraceMulti(
			Owner,
			Start,
			End,
			TraceChannel,
			bTraceComplex,
			ActorsToIgnore,
			DrawDebugType,
			OutHits,
			true,
			TraceColor,
			HitColor,
			DrawTime);

		for (const auto& Hit : OutHits)
		{
			if (!HitResult.ContainsByPredicate([&](const FHitResult& Inner) {  return Inner.GetActor() == Hit.GetActor(); }))
			{
				HitResult.Add(Hit);

				OnUniqueHit.Broadcast(Hit);
			}
		}
	}
}

void UHitTraceComponent::UpdateSocketLocation()
{
	for (auto Socket : Sockets)
	{
		SocketsLastLocations.Add(Socket, ReferenceMesh->GetSocketLocation(Socket));
	}
}

