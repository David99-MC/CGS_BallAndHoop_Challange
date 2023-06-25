// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "CGS_BallAndHoopCharacter.h"

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

	// ...
	if (Player)
	{
		PhysicHandleComp = Player->PhysicsHandle;
	}
	
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (PhysicHandleComp == nullptr) return;

	if (PhysicHandleComp->GetGrabbedComponent() != nullptr)
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * GrabDistance;
		PhysicHandleComp->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}

void UGrabber::Grab()
{
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * GrabDistance;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	DrawDebugSphere(GetWorld(), End, GrabRadius, 12, FColor::Green);

	FHitResult HitResult; // Out parameter
	bool bHasHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(GrabRadius));
	if (bHasHit)
	{
		if (PhysicHandleComp)
		{
			UPrimitiveComponent* HitComponent = HitResult.GetComponent();
			HitComponent->WakeAllRigidBodies();

			PhysicHandleComp->GrabComponentAtLocationWithRotation(
				HitComponent,
				NAME_None,
				HitResult.ImpactPoint,
				GetComponentRotation());
		}
	}
}

void UGrabber::Release()
{
	if (PhysicHandleComp == nullptr) return;

	if (PhysicHandleComp->GetGrabbedComponent() != nullptr)
	{
		PhysicHandleComp->GetGrabbedComponent()->WakeAllRigidBodies();
		PhysicHandleComp->ReleaseComponent();
	}
}

