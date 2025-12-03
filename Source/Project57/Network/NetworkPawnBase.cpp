// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkPawnBase.h"
#include "../Project57.h"
#include "NetworkUtil.h"

// Sets default values
ANetworkPawnBase::ANetworkPawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANetworkPawnBase::BeginPlay()
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

// Called every frame
void ANetworkPawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANetworkPawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANetworkPawnBase::PostNetInit()
{
	NET_LOG(TEXT("Begin"));

	Super::PostNetInit();

	NET_LOG(TEXT("End"));
}

void ANetworkPawnBase::PossessedBy(AController* NewController)
{
	NET_LOG(TEXT("Begin"));

	AActor* OwnerActor = GetOwner();
	if (OwnerActor) {
		FString Temp = FString::Printf(TEXT("Owner : %s"), *OwnerActor->GetName());
		NET_LOG(Temp);
	}
	else
	{
		NET_LOG(TEXT("No Owner"));
	}

	Super::PossessedBy(NewController);

	OwnerActor = GetOwner();
	if (OwnerActor) {
		FString Temp = FString::Printf(TEXT("Owner : %s"), *OwnerActor->GetName());
		NET_LOG(Temp);
	}
	else
	{
		NET_LOG(TEXT("No Owner"));
	}

	NET_LOG(TEXT("End"));
}

void ANetworkPawnBase::OnRep_Owner()
{
	NET_LOG(TEXT("Begin"));

	AActor* OwnerActor = GetOwner();
	if (OwnerActor) {
		FString Temp = FString::Printf(TEXT("Owner : %s"), *OwnerActor->GetName());
		NET_LOG(Temp);
	}
	else
	{
		NET_LOG(TEXT("No Owner"));
	}

	Super::OnRep_Owner();

	OwnerActor = GetOwner();
	if (OwnerActor) {
		FString Temp = FString::Printf(TEXT("Owner : %s"), *OwnerActor->GetName());
		NET_LOG(Temp);
	}
	else
	{
		NET_LOG(TEXT("No Owner"));
	}

	NET_LOG(TEXT("End"));
}

