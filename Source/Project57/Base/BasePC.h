// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePC.generated.h"

class UInGameWidget;
/**
 * 
 */
UCLASS()
class PROJECT57_API ABasePC : public APlayerController
{
	GENERATED_BODY()

public:
	ABasePC();

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<UInGameWidget> InGameWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UInGameWidget> InGameWidgetObject;
};
