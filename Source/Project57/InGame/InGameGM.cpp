// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameGM.h"
#include "InGameGS.h"
#include "../Base/BaseCharacter.h"

#include "Kismet/GameplayStatics.h"

AInGameGM::AInGameGM()
{
}

void AInGameGM::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

APlayerController* AInGameGM::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	APlayerController* PC = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	return PC;
}

void AInGameGM::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	CheckSurvivorCount();
}

void AInGameGM::Logout(AController* Exiting)
{
	CheckSurvivorCount();
	Super::Logout(Exiting);
}

void AInGameGM::BeginPlay()
{
	Super::BeginPlay();
}

int32 AInGameGM::CheckSurvivorCount()
{
	int32 SurvivorCount = 0;
	int32 PlayerCount = 0;

	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		ABaseCharacter* Pawn = Cast<ABaseCharacter>((*Iter)->GetPawn());
		if(Pawn)
		{
			PlayerCount++;
			if(Pawn->CurrentHP > 0)
			{
				SurvivorCount++;
			}
		}
	}

	AInGameGS* GS = GetGameState<AInGameGS>();
	if (GS)
	{
		GS->UpdateSurvivorCount(SurvivorCount);
	}

	FTimerHandle EndTimer;
	if(PlayerCount > 1 && SurvivorCount == 1)
	{
		GetWorld()->GetTimerManager().SetTimer(
			EndTimer,
			FTimerDelegate::CreateLambda([this]()
				{
					GetWorld()->ServerTravel(TEXT("Lobby"));
				}),
			10.0f,
			false);
	}

	return SurvivorCount;
}