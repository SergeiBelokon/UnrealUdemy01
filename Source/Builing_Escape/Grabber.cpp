// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber is Active"));
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
       OUT PlayerViewPointLocation,
       OUT PlayerViewPointRotation
    );
    /*
    UE_LOG(LogTemp, Warning, TEXT("Location is %s, Rotation is %s"),
       *PlayerViewPointLocation.ToString(),
       *PlayerViewPointRotation.ToString()
    );
    */
    FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
    
    DrawDebugLine(
      GetWorld(),
      PlayerViewPointLocation,
      LineTraceEnd,
      FColor(255, 0, 0),
      false,
      0.f,
      0.f,
      10.f
    );
    
    FHitResult Hit;
    FCollisionQueryParams TraceParametrs(TEXT(""), false, GetOwner());
    
    GetWorld()->LineTraceSingleByObjectType(
        OUT Hit,
        PlayerViewPointLocation,
        LineTraceEnd,
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        TraceParametrs
    );
    
    AActor* HitActor = Hit.GetActor();
    if (HitActor)
    {
        FString HitObjectName = HitActor->GetName();
        UE_LOG(LogTemp, Warning, TEXT("We hit a %s"), *HitObjectName);
    }
    
    
}

