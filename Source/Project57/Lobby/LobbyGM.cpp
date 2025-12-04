// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGM.h"
#include "LobbyGS.h"
#include "Kismet/GameplayStatics.h"

ALobbyGM::ALobbyGM()
{
}

void ALobbyGM::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	UE_LOG(LogTemp, Warning, TEXT("%s, Option : %s"), *Address, *Options);
	//접근을 제한하려면
	//ErrorMessage = TEXT("아무말");
}

APlayerController* ALobbyGM::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	APlayerController* PC = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	return PC;
}

void ALobbyGM::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	CheckConnectionCount();
}

void ALobbyGM::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

void ALobbyGM::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		LeftTimerHandle,
		FTimerDelegate::CreateLambda([this]()
			{
				ALobbyGS* GS = GetGameState<ALobbyGS>();
				if (GS)
				{
					GS->LeftTime--;
					GS->OnRep_LeftTime();
					if (GS->LeftTime == 0 || GS->bIsStarted)
					{
						GetWorld()->GetTimerManager().ClearTimer(LeftTimerHandle);
					}
				}
			}),
		1.0f,
		true,
		0.0f);
}

void ALobbyGM::CheckConnectionCount()
{
	ALobbyGS* GS = GetGameState<ALobbyGS>();
	if (GS)
	{
		int32 TempCount = 0;
		for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
		{
			TempCount++;
		}
		//GS->ConnectionCount = UGameplayStatics::GetNumPlayerControllers(GetWorld());
		GS->ConnectionCount = TempCount;
		GS->OnRep_ConnectionCount();
	}
}
