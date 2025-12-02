// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePCM.h"
#include "BaseCharacter.h"

ABasePCM::ABasePCM()
{
}

void ABasePCM::UpdateCamera(float DeltaTime)
{
	Super::UpdateCamera(DeltaTime);

	ABaseCharacter* Pawn = Cast<ABaseCharacter>(GetOwningPlayerController()->GetPawn());

	if (Pawn)
	{
		float TargetFOV = Pawn->bIsIronSight ? IronsightFOV : NormalFOV;
		float CurrentFOV = FMath::FInterpTo(GetFOVAngle(), TargetFOV, DeltaTime, ZoomSpeed);
		SetFOV(CurrentFOV);
	}
}
