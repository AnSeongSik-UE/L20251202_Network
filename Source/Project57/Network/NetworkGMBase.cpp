// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkGMBase.h"
#include "../Project57.h"
#include "NetworkUtil.h"

void ANetworkGMBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	NET_LOG(TEXT("Begin"));

	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	NET_LOG(TEXT("End"));
}

APlayerController* ANetworkGMBase::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	NET_LOG(TEXT("Begin"));

	APlayerController* PC = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	NET_LOG(TEXT("End"));

	return PC;
}

void ANetworkGMBase::PostLogin(APlayerController* NewPlayer)
{
	NET_LOG(TEXT("Begin"));

	Super::PostLogin(NewPlayer);

	NET_LOG(TEXT("End"));
}

void ANetworkGMBase::BeginPlay()
{
	NET_LOG(TEXT("Begin"));

	Super::BeginPlay();

	NET_LOG(TEXT("End"));
}

void ANetworkGMBase::StartPlay()
{
	NET_LOG(TEXT("Begin"));

	Super::StartPlay();

	NET_LOG(TEXT("End"));
}
