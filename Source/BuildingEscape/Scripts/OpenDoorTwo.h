// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoorTwo.generated.h"

//Forward declarations
class AMyPlayerController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSecondDoorEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoorTwo : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpenDoorTwo();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OpenDoor();
	void CloseDoor();

	UPROPERTY(BlueprintAssignable)
		FSecondDoorEvent OnOpenRequest;
	UPROPERTY(BlueprintAssignable)
		FSecondDoorEvent OnCloseRequest;

private:
	UPROPERTY(EditAnywhere)
		float NeededMass = 30.f;
	UPROPERTY(EditAnywhere)
		ATriggerVolume* FirstPressurePlate = nullptr;
	UPROPERTY(EditAnywhere)
		ATriggerVolume* SecondPressurePlate = nullptr;
	UPROPERTY(VisibleAnywhere)
		bool NeedsTwoDoors = false;

	void CheckTriggerVolumes();
	bool TotalMassOnTrigger(ATriggerVolume* ThisPressurePlate, float& TotalMass, bool& Triggered);
	void ComposeAndSendString(float FirstTriggerAmount, float SecondTriggerAmount, float FirstTriggerNeeded, float SecondTriggerNeeded);

	float CurrentTotalMass = 0.f;
	bool IsOpened = false;

	AMyPlayerController* MyPlayer = nullptr;
};
