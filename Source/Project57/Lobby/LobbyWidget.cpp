// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "LobbyGS.h"

#include "Kismet/GameplayStatics.h"

void ULobbyWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &ULobbyWidget::Start);
	}

	if (ChatInput)
	{
		ChatInput->OnTextCommitted.AddDynamic(this, &ULobbyWidget::ProcessOnTextCommitted);
		ChatInput->OnTextChanged.AddDynamic(this, &ULobbyWidget::ProcessOnTextChanged);
	}

	ALobbyGS* GS = Cast<ALobbyGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		GS->OnChangeLeftTime.AddDynamic(this, &ULobbyWidget::UpdateLeftTime);
	}

}

void ULobbyWidget::Start()
{
	GetWorld()->ServerTravel(TEXT("InGame"));
}

void ULobbyWidget::ProcessOnTextCommitted(const FText& InText, ETextCommit::Type CommitMethod)
{
}

void ULobbyWidget::ProcessOnTextChanged(const FText& InText)
{
}

void ULobbyWidget::UpdateLeftTime(int32 InLeftTime)
{
	if (LeftTime)
	{
		FString Message = FString::Printf(TEXT("%d초 남음"), InLeftTime);
		LeftTime->SetText(FText::FromString(Message));
	}
}
