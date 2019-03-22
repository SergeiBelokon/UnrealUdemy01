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
    FindPhysicsComponent();
    FindInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (PhysicsHandle->GrabbedComponent)
    {
        PhysicsHandle->SetTargetLocation(ReachLineEnd());
    }
    
}

void UGrabber::FindPhysicsComponent()
{
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (PhysicsHandle == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("PhysicsHandle is not found"));
    }
}

void UGrabber::FindInputComponent()
{
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if (InputComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("InputComponent is Active"));
        InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("InputComponent is not found"));
    }
}

void UGrabber::Grab()
{
    UE_LOG(LogTemp, Warning, TEXT("Grag func is called"));
    auto HitResult = GetFirstPhysicsBodyInReach();
    auto ComponentToGrab = HitResult.GetComponent();
    auto ActorHit = HitResult.GetActor();
    
    if (ActorHit)
    {
        PhysicsHandle->GrabComponentAtLocationWithRotation(
            ComponentToGrab,
            NAME_None,
            ActorHit->GetActorLocation(),
            ActorHit->GetActorRotation()
       );
    }
}

void UGrabber::Release()
{
    UE_LOG(LogTemp, Warning, TEXT("Release func is called"));
    PhysicsHandle->ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
    
    FHitResult Hit;
    FCollisionQueryParams TraceParametrs(TEXT(""), false, GetOwner());
    
    GetWorld()->LineTraceSingleByObjectType(
        OUT Hit,
        ReachLineStart(),
        ReachLineEnd(),
        FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
        TraceParametrs
    );
    
    AActor* HitActor = Hit.GetActor();
    if (HitActor)
    {
        FString HitObjectName = HitActor->GetName();
        UE_LOG(LogTemp, Warning, TEXT("We hit a %s"), *HitObjectName);
    }
    return Hit;
}

FVector UGrabber::ReachLineStart()
{
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
        OUT PlayerViewPointLocation,
        OUT PlayerViewPointRotation
    );

    return PlayerViewPointLocation;
}

FVector UGrabber::ReachLineEnd()
{
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
    /*
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
     */
    return LineTraceEnd;
}
