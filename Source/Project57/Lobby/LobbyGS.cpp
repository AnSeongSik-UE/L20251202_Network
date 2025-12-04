// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGS.h"
#include "Net/UnrealNetwork.h"
#include "LobbyPC.h"
#include "LobbyWidget.h"

#include "Kismet/GameplayStatics.h"

void ALobbyGS::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyGS, LeftTime);
	DOREPLIFETIME(ALobbyGS, ConnectionCount);
	UE_LOG(LogTemp, Warning, TEXT("GetLifetimeReplicatedProps"));
}

void ALobbyGS::OnRep_LeftTime()
{
	//ALobbyPC* PC = Cast<ALobbyPC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	//if (PC && PC->LobbyWidgetObject)
	//{
	//	PC->LobbyWidgetObject->UpdateLeftTime(LeftTime);
	//}
	OnChangeLeftTime.Broadcast(LeftTime);
}

void ALobbyGS::OnRep_ConnectionCount()
{
	OnChangeConnectionCount.Broadcast(ConnectionCount);
}

void ALobbyGS::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("ALobbyGS::BeginPlay"));
	Super::BeginPlay();
}
