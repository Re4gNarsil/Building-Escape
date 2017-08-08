// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "ObjectGrabber.h"
#include "MyPlayerController.h"


// Sets default values for this component's properties
UObjectGrabber::UObjectGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UObjectGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicshandle();
	FindInput();

	MyPlayer = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());
}

// Called every frame
void UObjectGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent) { PhysicsHandle->SetTargetLocation(FindReach()); }

}

// find the Physics Handle that we (assumably) attached before runtime
void UObjectGrabber::FindPhysicshandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle != nullptr) {}
	else { UE_LOG(LogTemp, Error, TEXT("Object %s does not have its Physics Handle"), *GetOwner()->GetName()); }
}

//Find the input component, which is attached by default
void UObjectGrabber::FindInput()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent != nullptr) {
		InputComponent->BindAction("Grabbing", IE_Pressed, this, &UObjectGrabber::GrabObject);
		InputComponent->BindAction("Grabbing", IE_Released, this, &UObjectGrabber::DropObject);
	}
	else { UE_LOG(LogTemp, Error, TEXT("Object %s does not have its Input"), *GetOwner()->GetName()); }
}

//find the direction we are looking and calculate how far we can reach
FVector UObjectGrabber::FindReach() {
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;

	//find the direction the default pawn is looking, and calculate where we can reach to
	GetOwner()->GetActorEyesViewPoint(PlayerViewLocation, PlayerViewRotation); //This GIVES us values
	FVector LineEnd = PlayerViewLocation + (PlayerViewRotation.GetNormalized().Vector() * LineLength);
	return LineEnd;
}

//find and return one actor for us to grab ahold of
FHitResult UObjectGrabber::GrabFirstObject() {

	//Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	//Line-trace (AKA ray-cast) out as far as we can reach
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(Hit, GetOwner()->GetActorLocation(), FindReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters);
	return Hit;
}

//called whenever we press the right mouse button; attempts to perform a grab
void UObjectGrabber::GrabObject() {

	FHitResult HitObject = GrabFirstObject();

	if (HitObject.GetActor()) {
		AttachHandle(HitObject.GetActor()); 
		float ObjectMass = HitObject.GetActor()->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		if (ObjectMass != NULL) { if (MyPlayer) { MyPlayer->WeightData(ObjectMass); } }
		//ObjectHeld = HitObject.GetActor();
		//ObjectHeld->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepRelativeTransform, GetOwner()->GetFName());
	}
}

// attach a Physics Handle component to the actor GrabFirstObject located
void UObjectGrabber::AttachHandle(AActor* HeldObject) {
	auto PhysicsComp = Cast<UPrimitiveComponent>(HeldObject->GetRootComponent());

	//if we are trying to grab a legitimate actor that can be picked up we do so
	if (PhysicsComp != NULL && PhysicsComp->BodyInstance.bSimulatePhysics)
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(PhysicsComp, NAME_None, HeldObject->GetActorLocation(), HeldObject->GetActorRotation());
		OurDistance = HeldObject->GetActorLocation() - GetOwner()->GetActorLocation();
	}
}

// called when we release the right mouse button; we simply drop the actor
void UObjectGrabber::DropObject() {
	if (PhysicsHandle->GrabbedComponent) { PhysicsHandle->ReleaseComponent(); }
	if (MyPlayer) { MyPlayer->WeightData(0.f); }
	//if (ObjectHeld->GetParentActor() == GetOwner()) {
	//	ObjectHeld->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	//}
}
