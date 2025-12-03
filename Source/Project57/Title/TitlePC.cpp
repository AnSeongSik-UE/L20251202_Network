// Fill out your copyright notice in the Description page of Project Settings.


#include "TitlePC.h"
#include "TitleWidget.h"

void ATitlePC::BeginPlay()
{
	Super::BeginPlay();

	//1. 경로 설정
	FSoftClassPath TitleWidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/MyProject/Blueprints/Title/WBP_Title.WBP_Title_C'"));

	//2. #include "Path" --> class 정의
	UClass* WidgetClass = TitleWidgetClass.TryLoadClass<UTitleWidget>();

	if (WidgetClass)
	{	//new class
		TitleWidgetObject = CreateWidget<UTitleWidget>(this, WidgetClass);
		if (TitleWidgetObject)
		{
			TitleWidgetObject->AddToViewport();
		}
	}
}

void ATitlePC::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
