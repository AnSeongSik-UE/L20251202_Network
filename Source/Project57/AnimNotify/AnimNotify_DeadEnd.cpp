// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_DeadEnd.h"
#include "../Base/BaseCharacter.h"

FString UAnimNotify_DeadEnd::GetNotifyName_Implementation() const
{
	return TEXT("DeadEnd");
}

void UAnimNotify_DeadEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ABaseCharacter* Pawn = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if (Pawn)
	{
		Pawn->DoDeadEnd();
	}
}
