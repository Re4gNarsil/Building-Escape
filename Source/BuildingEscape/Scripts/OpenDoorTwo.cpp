// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoorTwo.h"
#include "MyPlayerController.h"

// Sets default values for this component's properties
UOpenDoorTwo::UOpenDoorTwo()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoorTwo::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner()->GetName() == "OpenabkeDoorTwo_BP2") { NeedsTwoDoors = true; }

	MyPlayer = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
}

// Called every frame
void UOpenDoorTwo::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckTriggerVolumes();
}

void UOpenDoorTwo::CheckTriggerVolumes() 
{
	int Plates = 0;
	float FirstTotalMass = 0.f, SecondTotalMass = 0.f; //Out float parmeter for TotalMassOnTrigger
	bool Triggered; //Out bool parameter for TotalMassOnTrigger

	ATriggerVolume* PlateImOn = nullptr;

	if (!FirstPressurePlate) { UE_LOG(LogTemp, Error, TEXT("First Pressure Plate is Missing")); }
	else {
		if (TotalMassOnTrigger(FirstPressurePlate, FirstTotalMass, Triggered)) { PlateImOn = FirstPressurePlate; }
		if (Triggered) { Plates++; }
	}

	if (!SecondPressurePlate) { UE_LOG(LogTemp, Error, TEXT("Second Pressure Plate is Missing")); }
	else {
		if (TotalMassOnTrigger(SecondPressurePlate, SecondTotalMass, Triggered)) { PlateImOn = SecondPressurePlate; }
		if (Triggered) { Plates++; }
	}

	if (PlateImOn != nullptr) {
		if ((FirstTotalMass + SecondTotalMass) != CurrentTotalMass) {
			//If NeedsTwoDoors is true we need to trigger multiple plates to open the door;
			//otherwise we have the option to trigger one of two plates

			if ((NeedsTwoDoors && Plates > 1) || (!NeedsTwoDoors && Plates > 0)) { if (!IsOpened) { OpenDoor(); } }
			else if (IsOpened) { CloseDoor(); }

			if (MyPlayer) {
				if (PlateImOn == FirstPressurePlate) { ComposeAndSendString(FirstTotalMass, SecondTotalMass, NeededMass, NeededMass); }
				else { ComposeAndSendString(SecondTotalMass, FirstTotalMass, NeededMass, NeededMass); }
			}
			CurrentTotalMass = (FirstTotalMass + SecondTotalMass);
		}
	}
}

bool UOpenDoorTwo::TotalMassOnTrigger(ATriggerVolume* ThisPressurePlate, float& TotalMass, bool& Triggered)
{
	float Mass = 0.f;
	bool OnThisPlate = false;

	//find all overlapping actors
	TArray<AActor*> OverlappingActors;
	ThisPressurePlate->GetOverlappingActors(OverlappingActors);

	//if they are of the correct type get their masses
	for (auto& Actor : OverlappingActors) {
		if (Actor->FindComponentByClass<UPrimitiveComponent>()) { Mass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass(); }
		if (Actor == Cast <AActor>(GetWorld()->GetFirstPlayerController()->GetPawn())) { OnThisPlate = true; }
	}

	TotalMass = Mass;
	if (TotalMass > NeededMass) { Triggered = true; }
	else { Triggered = false; }
	return OnThisPlate;
}

void UOpenDoorTwo::ComposeAndSendString(float FirstTriggerAmount, float SecondTriggerAmount, float FirstTriggerNeeded, float SecondTriggerNeeded) 
{
	FString FirstData = FString("Current Trigger\n" + FString::FromInt(FMath::RoundToInt(FirstTriggerAmount)));
	FirstData += FString(" / " + FString::FromInt(FMath::RoundToInt(FirstTriggerNeeded)));
	UE_LOG(LogTemp, Warning, TEXT("Hey you"));
	FString SecondData;
	if (NeedsTwoDoors) { SecondData = FString("&& Other Trigger\n" + FString::FromInt(FMath::RoundToInt(SecondTriggerAmount))); }
	else { SecondData = FString("|| Other Trigger\n" + FString::FromInt(FMath::RoundToInt(SecondTriggerAmount))); }
	SecondData += FString(" / " + FString::FromInt(FMath::RoundToInt(SecondTriggerNeeded)));

	MyPlayer->CurrentTriggerData(FirstData);
	//MyPlayer->OtherTriggerData(SecondData);
}

void UOpenDoorTwo::OpenDoor() {
	OnOpenRequest.Broadcast();
	IsOpened = true;
}

void UOpenDoorTwo::CloseDoor() {
	OnCloseRequest.Broadcast();
	IsOpened = false;
}
