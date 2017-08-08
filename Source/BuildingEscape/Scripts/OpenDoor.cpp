// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"
#include "MyPlayerController.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	MyPlayer = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (CurrentTotalMass != GetTotalMass()) {
		CurrentTotalMass = GetTotalMass();

		if (CurrentTotalMass > NeededMass) { if (!IsOpened) { OpenDoor(); } }
		else if (IsOpened) { CloseDoor(); }
		if (MyPlayer) { 
			FString FirstData = FString("Current Trigger\n" + FString::FromInt(FMath::RoundToInt(CurrentTotalMass)));
			FirstData += FString(" / " + FString::FromInt(FMath::RoundToInt(NeededMass)));

			MyPlayer->CurrentTriggerData(FirstData);
			//MyPlayer->OtherTriggerData(FString(""));
		} 
	}

}

void UOpenDoor::OpenDoor() {
	OnOpenRequest.Broadcast();
	IsOpened = true;
}

void UOpenDoor::CloseDoor() {
	OnCloseRequest.Broadcast();
	IsOpened = false;
}

float UOpenDoor::GetTotalMass() {
	
	float TotalMass = 0.f;
	//find all overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { 
		UE_LOG(LogTemp, Error, TEXT("Pressure Plate is Missing"));
		return TotalMass;
	}
	PressurePlate->GetOverlappingActors(OverlappingActors);
	
	//if they are of the correct type get their masses
	for (auto& Actor : OverlappingActors) {
		if (Actor->FindComponentByClass<UPrimitiveComponent>()) { TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass(); }
	}
	return TotalMass;
}