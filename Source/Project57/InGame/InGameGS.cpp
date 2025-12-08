// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameGS.h"
#include "Net/UnrealNetwork.h"

AInGameGS::AInGameGS()
{
}

void AInGameGS::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInGameGS, SurvivorCount);
}

void AInGameGS::OnRep_SurvivorCount()
{
	OnChangeSurvivorCount.Broadcast(SurvivorCount);
}
