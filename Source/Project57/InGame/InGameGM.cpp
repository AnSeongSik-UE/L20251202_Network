// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameGM.h"
#include "InGameGS.h"

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
	Super::Logout(Exiting);
}

void AInGameGM::BeginPlay()
{
	Super::BeginPlay();
}

void AInGameGM::CheckSurvivorCount()
{
	AInGameGS* GS = GetGameState<AInGameGS>();
	if (GS)
	{
		int32 TempCount = 0;
		for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
		{
			TempCount++;
		}
		//GS->ConnectionCount = UGameplayStatics::GetNumPlayerControllers(GetWorld());
		GS->SurvivorCount = TempCount;
		GS->OnRep_SurvivorCount();
	}
}