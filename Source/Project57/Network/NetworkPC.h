// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NetworkPC.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT57_API ANetworkPC : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	
};
