// Fill out your copyright notice in the Description page of Project Settings.


#include "CompetePlayerState.h"
#include "Net/UnrealNetwork.h"
#include <Compete/CompeteGameStateBase.h>

ACompetePlayerState::ACompetePlayerState()
{
	PlayerHits = 0;
}

void ACompetePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACompetePlayerState, PlayerHits);
}

void ACompetePlayerState::OnRep_PlayerHits()
{
	if (ACompeteGameStateBase * GS = GetWorld()->GetGameState<ACompeteGameStateBase>())
	{
		uint16 GSHits = GS->GetTotalHits();
		UE_LOG(LogTemp, Warning, TEXT("PlayerHits = %d TotalHits =%d"), PlayerHits, GSHits);
	}
}

void ACompetePlayerState::PlayerHit()
{//ON THE SERVER
	if (GetWorld()->IsServer())
	{
		PlayerHits++;
	}
}
