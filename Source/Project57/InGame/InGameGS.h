// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "GameFramework/GameStateBase.h"
#include "InGameGS.generated.h"

DECLARE_DELEGATE_OneParam(FOnChangedSurvivorCount, const int32);

/**
 * 
 */
UCLASS()
class PROJECT57_API AInGameGS : public AGameStateBase
{
	GENERATED_BODY()
public:
	AInGameGS();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_SurvivorCount();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", ReplicatedUsing = "OnRep_SurvivorCount")
	int32 SurvivorCount = 100;
	
	FOnChangedSurvivorCount OnChangeSurvivorCount;

	void UpdateSurvivorCount(int32 InSurvivorCount);

};
