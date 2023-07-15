// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/EntityActor.h"
#include "Item/ItemObject.h"
#include "PropertyFragment/PropertyFragment_EntityLink.h"

// Sets default values
AEntityActor::AEntityActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AEntityActor::BeginPlay()
{
	if (bPrimitiveActor && ItemObject == nullptr)
	{
		ItemObject = NewObject<UItemObject>(this);
		Link = NewObject<UPropertyFragment_EntityLink>(this);
		Link->EntityType = GetClass();
		Link->Entity = this;
		ItemObject->AddPropertyFragment(Link);
	}

	Super::BeginPlay();
}

void AEntityActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	ItemObject = nullptr;
}