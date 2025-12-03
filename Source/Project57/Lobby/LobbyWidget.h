// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

class UButton;
class UEditableTextBox;
class UTextBlock;
class UScrollBox;
/**
 * 
 */
UCLASS()
class PROJECT57_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeOnInitialized() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (WidgetBind))
	TObjectPtr<UButton> StartButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (WidgetBind))
	TObjectPtr<UEditableTextBox> ChatInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (WidgetBind))
	TObjectPtr<UTextBlock> ConnectionCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (WidgetBind))
	TObjectPtr<UTextBlock> LeftTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (WidgetBind))
	TObjectPtr<UScrollBox> ChatScrollBox;

	UFUNCTION()
	void Start();

	UFUNCTION()
	void ProcessOnTextCommitted(const FText& InText, ETextCommit::Type CommitMethod);

	UFUNCTION()
	void ProcessOnTextChanged(const FText& InText);

	UFUNCTION(BlueprintCallable)
	void UpdateLeftTime(int32 InLeftTime);
};
