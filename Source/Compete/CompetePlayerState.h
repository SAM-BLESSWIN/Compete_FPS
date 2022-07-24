// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CompetePlayerState.generated.h"

/**
 * 
 */
UCLASS()
class COMPETE_API ACompetePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ACompetePlayerState();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_PlayerHits)
	uint16 PlayerHits;

	UFUNCTION()
	void OnRep_PlayerHits();

public:
	void PlayerHit();
};
