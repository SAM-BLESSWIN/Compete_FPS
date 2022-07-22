// Copyright Epic Games, Inc. All Rights Reserved.

#include "CompeteGameMode.h"
#include "CompeteCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACompeteGameMode::ACompeteGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
