// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"

void UTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	ServerIP = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ServerIP")));

	StartServerButton->OnClicked.AddDynamic(this, &UTitleWidget::StartServer);
	ConnectButton->OnClicked.AddDynamic(this, &UTitleWidget::Connect);
}

void UTitleWidget::StartServer()
{
}

void UTitleWidget::Connect()
{
}
