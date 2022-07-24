// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CompeteGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class COMPETE_API ACompeteGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	ACompeteGameStateBase();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_TotalHits)
	uint16 TotalHits;

	UFUNCTION()
		void OnRep_TotalHits();

public:
	void PlayerHit();
};
