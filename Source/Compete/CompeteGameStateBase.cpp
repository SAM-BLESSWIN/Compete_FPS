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
{//ON THE CLIENT
	UE_LOG(LogTemp, Warning, TEXT("Client : TotalHits = %d"), TotalHits);
}

void ACompeteGameStateBase::PlayerHit()
{
	if (HasAuthority())
	{//ON THE SERVER
		UE_LOG(LogTemp, Warning, TEXT("GameState:PlayerHit Called"));
		TotalHits++;
		UE_LOG(LogTemp, Warning, TEXT("Server : TotalHits = %d"), TotalHits);
	}
}
