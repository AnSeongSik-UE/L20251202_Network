// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"
#include "LobbyGS.h"
#include "LobbyPC.h"
#include "../Title/DataGameInstanceSubsystem.h"

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
		GS->OnChangeConnectionCount.AddDynamic(this, &ULobbyWidget::UpdateConnecionCount);
	}
	UE_LOG(LogTemp, Warning, TEXT("NativeOnInitialized"));

}

void ULobbyWidget::Start()
{
	ALobbyGS* GS = Cast<ALobbyGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (GS)
	{
		GS->bIsStarted = true;
		GetWorld()->ServerTravel(TEXT("InGame"));
	}
}

void ULobbyWidget::ProcessOnTextCommitted(const FText& InText, ETextCommit::Type CommitMethod)
{
	switch (CommitMethod)
	{
	case ETextCommit::OnEnter:
	{
		ALobbyPC* PC = Cast<ALobbyPC>(GetOwningPlayer());
		if (PC)
		{
			UGameInstance* GI = UGameplayStatics::GetGameInstance(GetWorld());
			if (GI)
			{
				UDataGameInstanceSubsystem* MySubsystem = GI->GetSubsystem<UDataGameInstanceSubsystem>();
				FString Temp = FString::Printf(TEXT("%s : %s"), *MySubsystem->UserID, *InText.ToString());
				
				//Local PC call -> Server PC execute
				PC->C2S_SendMessage(FText::FromString(Temp));

				ChatInput->SetText(FText::FromString(TEXT(""))); //지워지면 자동으로 OnCleared 실행
			}
		}
	}
		break;
	case ETextCommit::OnCleared:
	{
		ChatInput->SetUserFocus(GetOwningPlayer());
	}
		break;
	}
}

void ULobbyWidget::ProcessOnTextChanged(const FText& InText)
{
}

void ULobbyWidget::UpdateLeftTime(int32 InLeftTime)
{
	if (LeftTime)
	{
		if(InLeftTime >= 0)
		{
			FString Message = FString::Printf(TEXT("%d초 남음"), InLeftTime);
			LeftTime->SetText(FText::FromString(Message));
		}
		else
		{
			Start();
		}
	}
}

void ULobbyWidget::UpdateConnecionCount(int32 InConnectionCount)
{
	if (ConnectionCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateConnecionCount %d"), InConnectionCount);
		FString Message = FString::Printf(TEXT("%d명 접속"), InConnectionCount);
		ConnectionCount->SetText(FText::FromString(Message));
	}
}

void ULobbyWidget::AddMessage(const FText& Message)
{
	if (ChatScrollBox)
	{
		//UTextBlock* NewMessageBlock = NewObject<UTextBlock>(ChatScrollBox);
		//if(NewMessageBlock)
		//{
		//	NewMessageBlock->SetText(Message);
		//
		//	FSlateFontInfo FontInfo = NewMessageBlock->GetFont();
		//	FontInfo.Size = 20;
		//	NewMessageBlock->SetFont(FontInfo);
		//	NewMessageBlock->SetColorAndOpacity(FSlateColor(FLinearColor(0, 0, 1)));
		//	
		//	ChatScrollBox->AddChild(NewMessageBlock);
		//	ChatScrollBox->ScrollToEnd();
		//}

		URichTextBlock* NewMessageBlock = NewObject<URichTextBlock>(ChatScrollBox);
		if (NewMessageBlock)
		{
			NewMessageBlock->SetText(Message);
			NewMessageBlock->SetAutoWrapText(true);
			NewMessageBlock->SetWrapTextAt(ChatScrollBox->GetCachedGeometry().GetLocalSize().X);
			NewMessageBlock->SetWrappingPolicy(ETextWrappingPolicy::AllowPerCharacterWrapping);
			if (ChatStyleSet)
			{
				NewMessageBlock->SetTextStyleSet(ChatStyleSet);
			}
			ChatScrollBox->AddChild(NewMessageBlock);
			ChatScrollBox->ScrollToEnd();
		}
	}
}

void ULobbyWidget::ShowStartButton()
{
	if (StartButton)
	{
		StartButton->SetVisibility(ESlateVisibility::Visible);
	}
}
