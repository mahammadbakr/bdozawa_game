// Copyright Epic Games, Inc. All Rights Reserved.

#include "BdozawaCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Bdozawa.h"
#include "HideAndSeekCube.h"
#include "CylinderPickup.h"
#include "InputMappingContext.h"

ABdozawaCharacter::ABdozawaCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 2000.f;  // 2x faster jump (was 1000)
	GetCharacterMovement()->AirControl = 0.7f;  // Better air control for higher jumps
	GetCharacterMovement()->MaxWalkSpeed = 3000.f;  // 3x faster walk speed (was 1000)
	GetCharacterMovement()->MinAnalogWalkSpeed = 120.f;  // 3x minimum speed (was 40)
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	
	// Additional movement improvements for faster speeds
	GetCharacterMovement()->MaxAcceleration = 4096.f;  // Faster acceleration for quick starts
	GetCharacterMovement()->GroundFriction = 8.0f;  // Better ground control

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ABdozawaCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Setup Enhanced Input
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Add the default mapping context
			if (UInputMappingContext* DefaultMappingContext = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/Input/IMC_Default.IMC_Default")))
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
				UE_LOG(LogTemp, Warning, TEXT("Added IMC_Default mapping context"));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Could not load IMC_Default mapping context"));
			}
		}
	}
}

void ABdozawaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABdozawaCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ABdozawaCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABdozawaCharacter::Look);

		// Pickup
		EnhancedInputComponent->BindAction(PickupAction, ETriggerEvent::Started, this, &ABdozawaCharacter::DoPickup);
	}
	else
	{
		UE_LOG(LogBdozawa, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ABdozawaCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void ABdozawaCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ABdozawaCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void ABdozawaCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ABdozawaCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void ABdozawaCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void ABdozawaCharacter::DoPickup()
{
	// Find nearby pickup objects
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("World is null in DoPickup!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("E KEY PRESSED - Looking for pickup objects..."));

	// Get all actors in a sphere around the player
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, AActor::StaticClass());

	UE_LOG(LogTemp, Warning, TEXT("Found %d overlapping actors"), OverlappingActors.Num());

	// Look for HideAndSeekCube objects and CylinderPickup objects
	bool bFoundPickup = false;
	for (AActor* Actor : OverlappingActors)
	{
		// Check for HideAndSeekCube
		AHideAndSeekCube* Cube = Cast<AHideAndSeekCube>(Actor);
		if (Cube && !Cube->IsPickedUp())
		{
			UE_LOG(LogTemp, Warning, TEXT("Found HideAndSeekCube! Picked up: %s"), Cube->IsPickedUp() ? TEXT("YES") : TEXT("NO"));
			
			// Pick up the cube
			Cube->PickupCube();
			bFoundPickup = true;
			
			// Print success message
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("🎯 Picked up cube with E key!"));
			}
			break; // Only pick up one object at a time
		}
		
		// Check for CylinderPickup
		ACylinderPickup* Cylinder = Cast<ACylinderPickup>(Actor);
		if (Cylinder && !Cylinder->IsPickedUp())
		{
			UE_LOG(LogTemp, Warning, TEXT("Found CylinderPickup: %s! Picked up: %s"), *Cylinder->GetCylinderName(), Cylinder->IsPickedUp() ? TEXT("YES") : TEXT("NO"));
			
			// Pick up the cylinder
			Cylinder->PickupCylinder();
			bFoundPickup = true;
			
			// Print success message with cylinder name
			FString PickupMessage = FString::Printf(TEXT("🎯 %s picked up with E key!"), *Cylinder->GetCylinderName());
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, PickupMessage);
			}
			break; // Only pick up one object at a time
		}
	}

	if (!bFoundPickup)
	{
		UE_LOG(LogTemp, Warning, TEXT("No pickupable objects found nearby"));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("❌ No objects nearby to pick up!"));
		}
	}
}
