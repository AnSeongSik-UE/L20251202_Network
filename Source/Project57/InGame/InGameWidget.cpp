// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "../Base/BaseCharacter.h"
#include "InGameGS.h"
#include "InGameGM.h"

#include "Kismet/GameplayStatics.h"

#include "../Network/NetworkUtil.h"
#include "../Project57.h"

void UInGameWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AInGameGS* GS = Cast<AInGameGS>(GetWorld()->GetGameState());
	if (GS)
	{
		GS->OnChangeSurvivorCount.BindUObject(this, &UInGameWidget::ProcessChangeSurvivorCount);
	}

	AInGameGM* GM = Cast<AInGameGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM)
	{
		GM->CheckSurvivorCount();
	}

	APlayerController* PC = Cast<APlayerController>(GetOwningPlayer());
	if (PC)
	{
		ABaseCharacter* Pawn = Cast<ABaseCharacter>(PC->GetPawn());
		if(Pawn)
		{
			Pawn->OnCalculateHP.AddDynamic(this, &UInGameWidget::ProcessHPBar);
			Pawn->OnRep_CurrentHP();
		}
	}
	
}

void UInGameWidget::ProcessHPBar(float HPPercent)
{
	if (HPBar)
	{
		HPBar->SetPercent(HPPercent);
	}
}

void UInGameWidget::ProcessChangeSurvivorCount(int32 InSurvivorCount)
{
	if (SurvivorCount)
	{
		FString CurrentSurvivorText = FString::Printf(TEXT("%d명 생존"), InSurvivorCount);
		SurvivorCount->SetText(FText::FromString(CurrentSurvivorText));
	}
}
