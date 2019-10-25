// Copyright Alexander Garuba 2019.


#include "OpenDoor.h"
#define OUT

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
    Owner = GetOwner();
    if (!PressurePlate)
    {
        UE_LOG(LogTemp,Warning, TEXT("%s missing PressurePlate"), *GetOwner()->GetName()); // nullptr protect
    }
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Poll trigger volume
    if(GetTotalMassOfActorsOnPlate() > RequiredWeight)
    {
        OnOpen.Broadcast();
    }
    else
    {
        OnClose.Broadcast();
    }
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
    float TotalMass = 0.f;
    
    if (PressurePlate) // nullptr protect
    {
        /// find overlapping actors
        TArray<AActor*> OverlappingActors;
        PressurePlate->GetOverlappingActors(OUT OverlappingActors);
        
        /// iterate over actors and add masses
        for (const auto* Actor : OverlappingActors)
        {
            //UE_LOG(LogTemp,Warning, TEXT("%s on pressure plate"), *Actor->GetName());
            TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
        }
    }
    return TotalMass;
}
