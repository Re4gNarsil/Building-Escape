// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BUILDINGESCAPE_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
		FString GetTriggersData();
	UFUNCTION(BlueprintCallable, Category = "UI")
		FString GetExtraTriggersData();
	UFUNCTION(BlueprintCallable, Category = "UI")
		FString GetWeightData();

	void CurrentTriggerData(FString Data);
	void OtherTriggerData(FString Data);
	void WeightData(float Weight);

	FString CurrentData = "";
	FString OtherData = "";

private:
	float CurrentTriggerAmount = 0.f;
	float CurrentNeededAmount = 0.f;
	float ExtraTriggerAmount = 0.f;
	float ExtraNeededAmount = 0.f;
	float CurrentWeight = 0.f;

};
