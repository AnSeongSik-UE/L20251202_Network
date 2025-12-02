// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_GetDistance.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"

UBTService_GetDistance::UBTService_GetDistance()
{
	NodeName = TEXT("GetDistance");
}

void UBTService_GetDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));

	if (Target)
	{
		FVector ZombieLocation = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
		FVector PlayerLocation = Target->GetActorLocation();
		float Distance = FVector::Dist2D(ZombieLocation, PlayerLocation);

		UE_LOG(LogTemp, Warning, TEXT("UBTService_GetDistance %f"), Distance);

	}
}
