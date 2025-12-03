// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPC.h"
#include "LobbyWidget.h"
#include "LobbyGS.h"

#include "Kismet/GameplayStatics.h"

ALobbyPC::ALobbyPC()
{
}

void ALobbyPC::BeginPlay()
{
	Super::BeginPlay();

	if (LobbyWidgetClass)
	{
		if(IsLocalPlayerController())
		{
			LobbyWidgetObject = CreateWidget<ULobbyWidget>(this, LobbyWidgetClass);
			LobbyWidgetObject->AddToViewport();
		}
	}
}

void ALobbyPC::Tick(float Deltaseconds)
{
	Super::Tick(Deltaseconds);

	//ALobbyGS* GS = Cast<ALobbyGS>(UGameplayStatics::GetGameState(GetWorld()));

	//if (GS && LobbyWidgetObject)
	//{
	//	LobbyWidgetObject->UpdateLeftTime(GS->LeftTime);
	//}
}
