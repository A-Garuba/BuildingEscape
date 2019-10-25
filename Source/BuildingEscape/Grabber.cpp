// Copyright Alexander Garuba 2019.


#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
    FindPhysicsHandleComponent();
    SetupInputComponent();
}

void UGrabber::FindPhysicsHandleComponent()
{
    /// look for attached Physics handle
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (PhysicsHandle == nullptr)
    {
        UE_LOG(LogTemp,Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName());
    }
}

void UGrabber::SetupInputComponent()
{
    /// look for attached Input Component (only appears at run time)
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if (InputComponent)
    {
        InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
    }
    else
    {
        UE_LOG(LogTemp,Error, TEXT("%s missing input component"), *GetOwner()->GetName());
    }
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    if (!PhysicsHandle) { return; } //nullptr protect
    // if physics handle attached
    if (PhysicsHandle->GrabbedComponent)
    {
        // move the object that we're holding
        PhysicsHandle->SetTargetLocation(GetReachLineEnd());
    }
}

void UGrabber::Grab()
{
    /// LINE TRACE and see if we reach any actors with collision channel == PhysicsBody
    auto HitResult = GetFirstPhysicsBodyInReach();
    auto ComponentToGrab = HitResult.GetComponent(); // gets the mesh in our case
    auto ActorHit = HitResult.GetActor();
    
    if (ActorHit)
    {
        if (!PhysicsHandle) { return; } //nullptr protect
        PhysicsHandle->GrabComponentAtLocation(
                                     ComponentToGrab,
                                     NAME_None, // no bones needed
                                     ComponentToGrab->GetOwner()->GetActorLocation()
        );
    }
}

void UGrabber::Release()
{
    if (!PhysicsHandle) { return; } //nullptr protect
    PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
    /// Line-trace (ray-cast) out to reach distance
    FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
    
    FHitResult HitResult;
    GetWorld()->LineTraceSingleByObjectType(
                                            OUT HitResult,
                                            GetReachLineStart(),
                                            GetReachLineEnd(),
                                            FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
                                            TraceParameters
    );
    return HitResult;
}

FVector UGrabber::GetReachLineStart() const
{
    /// Get player view point
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
           OUT PlayerViewPointLocation,
           OUT PlayerViewPointRotation);
    
    return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd() const
{
    /// Get player view point
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
           OUT PlayerViewPointLocation,
           OUT PlayerViewPointRotation);
    
    return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}
