// Copyright Epic Games, Inc. All Rights Reserved.

#include "CGS_BallAndHoopCharacter.h"
#include "CGS_BallAndHoopProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/World.h"
#include "Grabber.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"


//////////////////////////////////////////////////////////////////////////
// ACGS_BallAndHoopCharacter

ACGS_BallAndHoopCharacter::ACGS_BallAndHoopCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	Grabber = CreateDefaultSubobject<UGrabber>(TEXT("Grabber Component"));
	Grabber->SetupAttachment(FirstPersonCameraComponent);

	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("Physics Handle Component"));
}

void ACGS_BallAndHoopCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

}

void ACGS_BallAndHoopCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Grabber->Player = this;
}

//////////////////////////////////////////////////////////////////////////// Input

void ACGS_BallAndHoopCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACGS_BallAndHoopCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACGS_BallAndHoopCharacter::Look);
	
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ACGS_BallAndHoopCharacter::Interact);
	}
}


void ACGS_BallAndHoopCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ACGS_BallAndHoopCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACGS_BallAndHoopCharacter::Interact(const FInputActionValue& Value)
{
	bool bIsPressed = Value.Get<bool>();
	if (bIsPressed)
	{
		Grabber->Grab();
	}
	else
	{
		Grabber->Release();
	}
}

void ACGS_BallAndHoopCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool ACGS_BallAndHoopCharacter::GetHasRifle()
{
	return bHasRifle;
}