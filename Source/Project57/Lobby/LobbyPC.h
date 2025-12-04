// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPC.generated.h"

class ULobbyWidget;
class UUserWidget;
/**
 * 
 */
UCLASS()
class PROJECT57_API ALobbyPC : public APlayerController
{
	GENERATED_BODY()
public:
	ALobbyPC();

	virtual void BeginPlay()override;

	virtual void Tick(float Deltaseconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<ULobbyWidget> LobbyWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<ULobbyWidget> LobbyWidgetObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<UUserWidget> LoadingWidgetClass;

	//자동으로 네트워크를 통해서 실행하는 코드 생성
	UFUNCTION(Server, Reliable, WithValidation)
	void C2S_SendMessage(const FText& Message);					//네트워크로 던지는 코드
	bool C2S_SendMessage_Validate(const FText& Message);		//서버에서 확인하는 코드
	void C2S_SendMessage_Implementation(const FText& Message);	//실제동작

	UFUNCTION(Client, Reliable)
	void S2C_SendMessage(const FText& Message);					//네트워크 정보 던져서 실행, 자동생성
	void S2C_SendMessage_Implementation(const FText& Message);	//실제동작

	UFUNCTION(Client, Reliable)
	void S2C_ShowLoadingScreen();
	void S2C_ShowLoadingScreen_Implementation();
};
