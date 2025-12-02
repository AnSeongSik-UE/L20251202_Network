// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetRandomPoint.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetRandomPoint::UBTTask_SetRandomPoint()
{
	NodeName = "SetRandomPoint";
}

EBTNodeResult::Type UBTTask_SetRandomPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = Cast<AAIController>(OwnerComp.GetAIOwner());
	if (AIC)
	{
		FVector Location = AIC->GetPawn()->GetActorLocation();
		FVector Destination;
		
		if (UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(
			GetWorld(),
			Location,
			Destination,
			5000.0f
		))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(
				GetSelectedBlackboardKey(),
				Destination
			);

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
