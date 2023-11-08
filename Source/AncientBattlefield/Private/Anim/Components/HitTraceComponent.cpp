// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/Components/HitTraceComponent.h"

// Sets default values for this component's properties
UHitTraceComponent::UHitTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TraceChannel = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Pawn);
	bTraceComplex = false;
	ActorsToIgnore.Empty();
	DrawDebugType = EDrawDebugTrace::None;
	TraceColor = FLinearColor::Yellow;
	HitColor = FLinearColor::Red;
	DrawTime = 0.2f;
}


// Called when the game starts
void UHitTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = nullptr;
	bTrace = false;
	SocketsLastLocations.Empty();
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

void UHitTraceComponent::Setup(UPrimitiveComponent* Reference, bool bSearchSocketsFromMesh)
{
	ReferenceMesh = Reference;
	Owner = GetOwner();
	ActorsToIgnore.Add(Owner);
	if (bSearchSocketsFromMesh)
	{
		Sockets = ReferenceMesh->GetAllSocketNames();
	}
}

void UHitTraceComponent::Teardown()
{
	ReferenceMesh = nullptr;
	Sockets.Empty();
}

void UHitTraceComponent::SetSockets(const TArray<FName>& SetSockets)
{
	Sockets = SetSockets;
}

void UHitTraceComponent::EnableTrace()
{
	if (ReferenceMesh == nullptr || Owner == nullptr)
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
	for (auto StartSocket : Sockets)
	{
		auto LastLocation = SocketsLastLocations.Find(StartSocket);
		if (LastLocation == nullptr)
		{
			continue;
		}
		const FVector Start = *LastLocation;
		for (auto EndSocket : Sockets)
		{
			TArray<FHitResult> OutHits;
			const FVector End = ReferenceMesh->GetSocketLocation(EndSocket);
			UKismetSystemLibrary::LineTraceMulti(Owner, Start, End, TraceChannel, bTraceComplex, ActorsToIgnore, DrawDebugType, OutHits, true, TraceColor, HitColor, DrawTime);

			for (const auto& Hit : OutHits)
			{
				if (!HitResult.ContainsByPredicate([&](const FHitResult& Inner) { return Inner.GetActor() == Hit.GetActor(); }))
				{
					HitResult.Add(Hit);
					UniqueHitDelegate.Broadcast(Hit);
				}
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

