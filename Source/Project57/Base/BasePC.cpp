// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePC.h"
#include "../InGame/InGameWidget.h"

#include "../Network/NetworkUtil.h"
#include "../Project57.h"

ABasePC::ABasePC()
{

}

void ABasePC::BeginPlay()
{
	Super::BeginPlay();

	if (InGameWidgetClass)
	{
		if (IsLocalController())
		{
			InGameWidgetObject = CreateWidget<UInGameWidget>(this, InGameWidgetClass);
			if(InGameWidgetObject)
			{
				InGameWidgetObject->AddToViewport();
			}
		}
	}
}
