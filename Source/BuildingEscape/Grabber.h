// Copyright Alexander Garuba 2019.

#pragma once

#include "BuildingEscape.h"
#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    float Reach = 200.f;
    
    UPhysicsHandleComponent* PhysicsHandle = nullptr;
    UInputComponent* InputComponent = nullptr;
    
    // Ray-cast and grab what's in reach
    void Grab();
    
    // Release what has been grabbed
    void Release();
    
    // Find Physics Handle component
    void FindPhysicsHandleComponent();
    
    // Create attached Input component
    void SetupInputComponent();
    
    // Return hit for first physics body in reach
    FHitResult GetFirstPhysicsBodyInReach() const;
    
    // Returns current start of reach line
    FVector GetReachLineStart() const;
    
    // Returns current end of reach line
    FVector GetReachLineEnd() const;
};
