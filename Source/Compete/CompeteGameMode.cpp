// Copyright Epic Games, Inc. All Rights Reserved.

#include "CompeteGameMode.h"
#include "CompeteCharacter.h"
#include "Compete/CompeteGameStateBase.h"
#include "UObject/ConstructorHelpers.h"

ACompeteGameMode::ACompeteGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}

void ACompeteGameMode::PlayerHit()
{
	if (ACompeteGameStateBase* GS = GetGameState<ACompeteGameStateBase>())
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode:PlayerHit Called"));
		GS->PlayerHit();
	}
}


