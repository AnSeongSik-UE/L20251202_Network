// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "GameFramework/GameStateBase.h"
#include "LobbyGS.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedLeftTime, const int32, InLeftTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedConnectionCount, const int32, InConnectionCount);

/**
 * 
 */
UCLASS()
class PROJECT57_API ALobbyGS : public AGameStateBase
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void OnRep_LeftTime();

	UFUNCTION()
	void OnRep_ConnectionCount();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", ReplicatedUsing = "OnRep_LeftTime")
	int32 LeftTime = 60;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", ReplicatedUsing = "OnRep_ConnectionCount")
	int32 ConnectionCount = 0;

	//CustomEvent ª°∞£¡Ÿ
	UPROPERTY(BlueprintAssignable)
	FOnChangedLeftTime OnChangeLeftTime;

	UPROPERTY(BlueprintAssignable)
	FOnChangedConnectionCount OnChangeConnectionCount;

	virtual void BeginPlay() override;
};
