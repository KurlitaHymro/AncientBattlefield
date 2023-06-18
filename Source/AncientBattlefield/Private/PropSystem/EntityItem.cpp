// Fill out your copyright notice in the Description page of Project Settings.


#include "PropSystem/EntityItem.h"
#include "Item/ItemObject.h"
#include "PropertyFragment/PropertyFragment_PropBaseInfo.h"

// Sets default values
AEntityItem::AEntityItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEntityItem::BeginPlay()
{
	Super::BeginPlay();

	ItemObject = NewObject<UItemObject>();
	for (auto PropertyFragment : GrantPropertyFragments)
	{
		ItemObject->ConstructPropertyFragment(PropertyFragment);
	}
	InitPropertyFragment();
}

void AEntityItem::InitPropertyFragment_Implementation()
{
}
