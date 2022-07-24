// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CompeteGameMode.generated.h"

UCLASS(minimalapi)
class ACompeteGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACompeteGameMode();
	void PlayerHit();
};



