// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "RotateObject.h"


// Sets default values for this component's properties
URotateObject::URotateObject()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URotateObject::BeginPlay()
{
	//Super::BeginPlay();

	// ...
	
}


// Called every frame
void URotateObject::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	//if (TriggeringActor != nullptr) {
	//	if (PressurePlate->IsOverlappingActor(TriggeringActor)) { if (!IsOpened) { OpenDoor(); } }
	//	else if (IsOpened) { CloseDoor(); }
	//}
	//else { TriggeringActor = GetWorld()->GetFirstPlayerController()->GetPawn(); }

	// ...
}

void URotateObject::OpenDoor()
{
	AActor* Owner = GetOwner();

	FRotator ObjectRotation = Owner->GetActorRotation();
	ObjectRotation.Yaw = ObjectRotation.Yaw + OpenAngle;
	Owner->SetActorRotation(ObjectRotation);
	IsOpened = true;
}

void URotateObject::CloseDoor() {
	GetOwner()->SetActorRotation(FRotator(0, StartAngle, 0));
	IsOpened = false;
}
