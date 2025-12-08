// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameWidget.generated.h"

class UTextBlock;
class UProgressBar;
/**
 * 
 */
UCLASS()
class PROJECT57_API UInGameWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeOnInitialized() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (WidgetBind))
	TObjectPtr<UTextBlock> SurvivorCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (WidgetBind))
	TObjectPtr<UProgressBar> HPBar;

	UFUNCTION()
	void ProcessHPBar(float HPPercent);

	void ProcessChangeSurvivorCount(int32 InSurvivorCount);
};
