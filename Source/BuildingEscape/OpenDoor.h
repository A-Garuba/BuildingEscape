// Copyright Alexander Garuba 2019.

#pragma once

#include "BuildingEscape.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    
    UPROPERTY(BlueprintAssignable)
    FDoorEvent OnOpen;
    
    UPROPERTY(BlueprintAssignable)
    FDoorEvent OnClose;

private:
    UPROPERTY(EditAnywhere)
    float OpenAngle = 90.0f;
    
    UPROPERTY(EditAnywhere)
    ATriggerVolume* PressurePlate = nullptr;
    
    UPROPERTY(EditAnywhere)
    float RequiredWeight = 50.f;
    
    // The door that has OpenDoor Component
    AActor* Owner = nullptr;
    
    // Returns total mass on plate in kg
    float GetTotalMassOfActorsOnPlate();
};
