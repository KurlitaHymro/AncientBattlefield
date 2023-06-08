// Fill out your copyright notice in the Description page of Project Settings.


#include "PropSystem/EntityItem.h"

// Sets default values
AEntityItem::AEntityItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ItemObject = 
}

// Called when the game starts or when spawned
void AEntityItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEntityItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

