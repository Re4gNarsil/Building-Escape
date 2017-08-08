// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "ObjectGrabber.generated.h"

//Forward declarations
class AMyPlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UObjectGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UObjectGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	float LineLength = 100.f;
	FVector OurDistance;

	AActor* ObjectHeld = nullptr;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	//used to find (assumed) handle component
	void FindPhysicshandle();

	//used to find (assumed) input component
	void FindInput();

	//this is how we find how far we can reach
	FVector FindReach();

	//called when we are searching for an object to grab
	FHitResult GrabFirstObject();

	//used each frame that we are trying to hold an object
	void GrabObject();

	//used to attach a physics handle to our grabbed object
	void AttachHandle(AActor* HeldObject);

	//used whenever we release an object
	void DropObject();

	AMyPlayerController* MyPlayer = nullptr;
};
