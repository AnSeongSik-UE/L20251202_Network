// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_LookPlayer.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/KismetMathLibrary.h"

UBTService_LookPlayer::UBTService_LookPlayer()
{
	NodeName = TEXT("LookPlayer");
}

void UBTService_LookPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AActor* Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (Player)
	{
		AActor* Zombie = OwnerComp.GetAIOwner()->GetPawn();
		FVector ZombieLocation = Zombie->GetActorLocation();
		FVector PlayerLocation = Player->GetActorLocation();

		FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(
			ZombieLocation,
			PlayerLocation
		);

		LookRotation.Pitch = 0;

		FRotator TargetRotation = FMath::RInterpTo(Zombie->GetActorRotation(),
			LookRotation, DeltaSeconds, 15.0f);

		Zombie->SetActorRotation(TargetRotation);

	}
}
