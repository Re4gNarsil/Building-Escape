// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "RotateObject.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API URotateObject : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URotateObject();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	void OpenDoor();

	void CloseDoor();

private:
	UPROPERTY(VisibleAnywhere)
		float OpenAngle = 77.5f;
	float StartAngle = 0;
	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate;
	AActor* TriggeringActor;
	bool IsOpened = false;

};
