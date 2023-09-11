// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/Components/TrailingComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values for this component's properties
UTrailingComponent::UTrailingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Template = nullptr;
	LocationOffset.Set(0.0f, 0.0f, 0.0f);
	RotationOffset = FRotator(0.0f, 0.0f, 0.0f);
}


// Called when the game starts
void UTrailingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTrailingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTrailingComponent::RibbonTrailingSetup(UPrimitiveComponent* Reference, bool bSearchSocketsFromMesh, FName Start, FName End)
{
	UMeshComponent* Mesh = CastChecked<UMeshComponent>(Reference);
	if (bSearchSocketsFromMesh)
	{
		auto Sockets = Mesh->GetAllSocketNames();
		if (Sockets.Num() >= 2)
		{
			Start = Sockets[0];
			End = Sockets[1];
		}
	}

	if (!Start.IsNone() && !End.IsNone())
	{
		SocketName = Start;
		auto StartLocation = Mesh->GetSocketTransform(Start, RTS_Actor).GetTranslation();
		auto EndLocation = Mesh->GetSocketTransform(End, RTS_Actor).GetTranslation();
		LocationOffset = (EndLocation - StartLocation) / 2;
		RotationOffset = UKismetMathLibrary::FindLookAtRotation(StartLocation, EndLocation);
		Setup(Mesh);
	}
}

void UTrailingComponent::Setup(UPrimitiveComponent* Reference)
{
	ReferenceMesh = Reference;
	Owner = GetOwner();
}

void UTrailingComponent::Teardown()
{
	ReferenceMesh = nullptr;
	Owner = nullptr;
} 

void UTrailingComponent::EnableTrailing()
{
	if (UFXSystemComponent* Component = SpawnEffect(ReferenceMesh))
	{
		// tag the component with the AnimNotify that is triggering the animation so that we can properly clean it up
		Component->ComponentTags.AddUnique(GetSpawnedComponentTag());
	}
}

void UTrailingComponent::DisableTrailing()
{
	if (UFXSystemComponent* FXComponent = GetSpawnedEffect())
	{
		// untag the component
		FXComponent->ComponentTags.Remove(GetSpawnedComponentTag());

		// Either destroy the component or deactivate it to have it's active FXSystems finish.
		// The component will auto destroy once all FXSystem are gone.
		FXComponent->Deactivate();
	}
}

UFXSystemComponent* UTrailingComponent::GetSpawnedEffect()
{
	if (ReferenceMesh)
	{
		TArray<USceneComponent*> Children;
		ReferenceMesh->GetChildrenComponents(false, Children);

		if (Children.Num())
		{
			for (USceneComponent* Component : Children)
			{
				if (Component && Component->ComponentHasTag(GetSpawnedComponentTag()))
				{
					if (UFXSystemComponent* FXComponent = CastChecked<UFXSystemComponent>(Component))
					{
						return FXComponent;
					}
				}
			}
		}
	}

	return nullptr;
}

UFXSystemComponent* UTrailingComponent::SpawnEffect(USceneComponent* MeshComp) const
{
	// Only spawn if we've got valid params
	if (Template)
	{
		return UNiagaraFunctionLibrary::SpawnSystemAttached(Template, MeshComp, SocketName, LocationOffset, RotationOffset, EAttachLocation::KeepRelativeOffset, true);
	}
	return nullptr;
}

