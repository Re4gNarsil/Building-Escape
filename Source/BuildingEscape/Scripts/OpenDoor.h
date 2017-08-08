// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"

//Forward declarations
class AMyPlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	void OpenDoor();
	void CloseDoor();

	UPROPERTY(BlueprintAssignable)
		FDoorEvent OnOpenRequest;
	UPROPERTY(BlueprintAssignable)
		FDoorEvent OnCloseRequest;

private:
	UPROPERTY(EditAnywhere)
		float NeededMass = 30.f;
	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate = nullptr;
	bool IsOpened = false;
	float GetTotalMass();
	float CurrentTotalMass = 0.f;

	AMyPlayerController* MyPlayer = nullptr;
};
