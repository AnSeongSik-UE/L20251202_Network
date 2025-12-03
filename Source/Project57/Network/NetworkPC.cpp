// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkPC.h"
#include "../Project57.h"
#include "NetworkUtil.h"

void ANetworkPC::BeginPlay()
{
	NET_LOG(TEXT("Begin"));

	Super::BeginPlay();

	if (GetLocalRole() == ENetRole::ROLE_Authority)
	{
		NET_LOG(TEXT("LocalRole::ROLE_Authority"));
	}
	else if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
	{
		NET_LOG(TEXT("LocalRole::ROLE_AutonomousProxy"));
	}
	else if (GetLocalRole() == ENetRole::ROLE_SimulatedProxy)
	{
		NET_LOG(TEXT("LocalRole::ROLE_SimulatedProxy"));
	}

	if (GetRemoteRole() == ENetRole::ROLE_Authority)
	{
		NET_LOG(TEXT("RemoteRole::ROLE_Authority"));
	}
	else if (GetRemoteRole() == ENetRole::ROLE_AutonomousProxy)
	{
		NET_LOG(TEXT("RemoteRole::ROLE_AutonomousProxy"));
	}
	else if (GetRemoteRole() == ENetRole::ROLE_SimulatedProxy)
	{
		NET_LOG(TEXT("RemoteRole::ROLE_SimulatedProxy"));
	}
	NET_LOG(TEXT("End"));
}

void ANetworkPC::PostNetInit()
{
	NET_LOG(TEXT("Begin"));

	Super::PostNetInit();

	NET_LOG(TEXT("End"));
}
