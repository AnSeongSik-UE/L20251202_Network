// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "BasePCM.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT57_API ABasePCM : public APlayerCameraManager
{
	GENERATED_BODY()

public:

	ABasePCM();

	virtual void UpdateCamera(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float NormalFOV = 90.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float IronsightFOV = 60.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
	float ZoomSpeed = 15.0f;
	
};
