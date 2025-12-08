// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "../Base/BaseCharacter.h"
#include "InGameGS.h"

#include "Kismet/GameplayStatics.h"

#include "../Network/NetworkUtil.h"
#include "../Project57.h"

void UInGameWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AInGameGS* GS = Cast<AInGameGS>(GetWorld()->GetGameState());
	if (GS)
	{
		GS->OnChangeSurvivorCount.AddDynamic(this, &UInGameWidget::UpdateSurvivorCount);
	}

	ABaseCharacter* Character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn());
	if (Character)
	{
		Character->OnCalculateHP.BindUObject(this, &UInGameWidget::SetHPBar);
	}
	
}

void UInGameWidget::SetHPBar(float HPPercent)
{
	if (HPBar)
	{
		HPBar->SetPercent(HPPercent);
	}
}

void UInGameWidget::UpdateSurvivorCount(int32 InSurvivorCount)
{
	if (SurvivorCount)
	{
		FString CurrentSurvivorText = FString::Printf(TEXT("%d명 남음"), InSurvivorCount);
		SurvivorCount->SetText(FText::FromString(CurrentSurvivorText));
	}
}
