// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PROJECT57_API NetworkUtil
{
public:
	NetworkUtil();
	~NetworkUtil();

	static void Log(UWorld* world, const FString& InString);
};
