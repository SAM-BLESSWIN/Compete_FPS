// Fill out your copyright notice in the Description page of Project Settings.


#include "CompeteGameStateBase.h"
#include "Net/UnrealNetwork.h"

ACompeteGameStateBase::ACompeteGameStateBase()
{
	TotalHits = 0;
}

void ACompeteGameStateBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACompeteGameStateBase, TotalHits);
}

void ACompeteGameStateBase::OnRep_TotalHits()
{
	UE_LOG(LogTemp, Warning, TEXT("Total Hits : %d"),TotalHits);
}

void ACompeteGameStateBase::PlayerHit()
{//ON THE SERVER
	if (GetWorld()->IsServer())
	{
		UE_LOG(LogTemp, Warning, TEXT("GameState [Server]"));
		TotalHits++;         //Make OnRep_TotalHits() call on the Clients
		//OnRep_TotalHits(); //Make OnRep_TotalHits() call on the Server
	}
}

uint16 ACompeteGameStateBase::GetTotalHits() const
{
	return TotalHits;
}
