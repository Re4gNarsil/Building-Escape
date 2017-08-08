// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "MyPlayerController.h"


FString AMyPlayerController::GetTriggersData()
{
	return CurrentData;
}

FString AMyPlayerController::GetExtraTriggersData() //only displays something if we are triggering a multi-volume trigger
{
	return OtherData;
}

FString AMyPlayerController::GetWeightData()
{
	FString Data = FString("Object Weighs\n" + FString::FromInt(FMath::RoundToInt(CurrentWeight)));
	return Data;
}

void AMyPlayerController::CurrentTriggerData(FString Data)
{
	CurrentData = Data;
}

void AMyPlayerController::OtherTriggerData(FString Data)
{
	OtherData = Data;
}

void AMyPlayerController::WeightData(float Weight)
{
	CurrentWeight = Weight;
}
