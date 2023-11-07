// Copyright Epic Games, Inc. All Rights Reserved.

#include "AncientBattlefieldGameMode.h"
#include "AncientBattlefieldCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAncientBattlefieldGameMode::AAncientBattlefieldGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
