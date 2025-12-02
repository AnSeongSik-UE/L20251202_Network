// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkPC.h"
#include "../Project57.h"
#include "NetworkUtil.h"

void ANetworkPC::BeginPlay()
{
	NET_LOG(TEXT("Begin"));

	Super::BeginPlay();

	NET_LOG(TEXT("End"));
}
