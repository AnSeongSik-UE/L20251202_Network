// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_DeadEnd.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT57_API UAnimNotify_DeadEnd : public UAnimNotify
{
	GENERATED_BODY()
	
	virtual FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
