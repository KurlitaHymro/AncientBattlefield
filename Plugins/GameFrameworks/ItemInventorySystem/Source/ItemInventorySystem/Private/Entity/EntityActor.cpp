// Fill out your copyright notice in the Description page of Project Settings.


#include "Entity/EntityActor.h"
#include "Item/ItemObject.h"
#include "PropertyFragment/PropertyFragment_EntityLink.h"

void AEntityActor::CreateItem_Implementation()
{
	ItemObject = NewObject<UItemObject>(this);

	Link = NewObject<UPropertyFragment_EntityLink>(this);
	Link->EntityType = GetClass();
	Link->Entity = this;
	ItemObject->AddPropertyFragment(Link);
}

void AEntityActor::ResetItem_Implementation()
{
	Link->Entity = this;
}