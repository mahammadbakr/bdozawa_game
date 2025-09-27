// Copyright Epic Games, Inc. All Rights Reserved.

#include "HideAndSeekCube.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/Engine.h"
#include "BdozawaCharacter.h"

AHideAndSeekCube::AHideAndSeekCube()
{
	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	// Create the static mesh component
	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	RootComponent = CubeMesh;
	
	// Make the cube larger and more visible
	CubeMesh->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));

	// Create the pickup sphere component
	PickupSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickupSphere"));
	PickupSphere->SetupAttachment(RootComponent);
	PickupSphere->SetSphereRadius(100.0f); // 1 meter radius for pickup
	PickupSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickupSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	PickupSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// Initialize variables
	bIsPickedUp = false;
}

void AHideAndSeekCube::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind overlap events - use a custom function with correct signature
	PickupSphere->OnComponentBeginOverlap.AddDynamic(this, &AHideAndSeekCube::OnOverlapBegin);
	PickupSphere->OnComponentEndOverlap.AddDynamic(this, &AHideAndSeekCube::OnOverlapEnd);
	
	// Start a timer to continuously show pickup icon when near player
	GetWorldTimerManager().SetTimer(PickupIconTimer, this, &AHideAndSeekCube::UpdatePickupIcon, 0.1f, true);
}

void AHideAndSeekCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHideAndSeekCube::PickupCube()
{
	if (!bIsPickedUp)
	{
		bIsPickedUp = true;
		
		// Hide the cube completely
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);
		
		// Hide pickup icon
		HidePickupIcon();
		
		// Call Blueprint event
		OnCubePickedUp();
		
		// Print debug message
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("🎯 CUBE PICKED UP!"));
		}
		
		// Log to console
		UE_LOG(LogTemp, Warning, TEXT("CUBE PICKED UP at location: %s"), *GetActorLocation().ToString());
		
	}
}

void AHideAndSeekCube::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the overlapping actor is the player character
	ABdozawaCharacter* PlayerCharacter = Cast<ABdozawaCharacter>(OtherActor);
	if (PlayerCharacter && !bIsPickedUp)
	{
		// Show pickup icon
		ShowPickupIcon();
		
		// Call Blueprint event for player entering range
		OnPlayerEnterRange();
		
		// Don't auto-pickup, let player press E
		UE_LOG(LogTemp, Warning, TEXT("Player entered pickup range - showing icon"));
	}
}

void AHideAndSeekCube::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Check if the leaving actor is the player character
	ABdozawaCharacter* PlayerCharacter = Cast<ABdozawaCharacter>(OtherActor);
	if (PlayerCharacter && !bIsPickedUp)
	{
		// Hide pickup icon
		HidePickupIcon();
		
		// Call Blueprint event for player leaving range
		OnPlayerLeaveRange();
		
		UE_LOG(LogTemp, Warning, TEXT("Player left pickup range - hiding icon"));
	}
}

void AHideAndSeekCube::NotifyActorBeginOverlap(AActor* OtherActor)
{
	// This function is kept for compatibility but we use OnOverlapBegin instead
	// Check if the overlapping actor is the player character
	ABdozawaCharacter* PlayerCharacter = Cast<ABdozawaCharacter>(OtherActor);
	if (PlayerCharacter && !bIsPickedUp)
	{
		// Call Blueprint event for player entering range
		OnPlayerEnterRange();
		
		// Auto-pickup when player gets close
		PickupCube();
	}
}

void AHideAndSeekCube::ShowPickupIcon()
{
	if (!bPickupIconVisible && !bIsPickedUp)
	{
		bPickupIconVisible = true;
		
		// Draw a hand icon above the cube
		if (UWorld* World = GetWorld())
		{
			// Draw a hand-like shape using debug lines
			FVector CubeLocation = GetActorLocation();
			FVector IconLocation = CubeLocation + FVector(0, 0, 200); // Above the cube
			
			// Draw hand icon (simple representation) - make it more visible
			DrawDebugLine(World, IconLocation + FVector(-30, 0, 0), IconLocation + FVector(30, 0, 0), FColor::Yellow, true, 0.1f, 0, 5.0f);
			DrawDebugLine(World, IconLocation + FVector(0, -30, 0), IconLocation + FVector(0, 30, 0), FColor::Yellow, true, 0.1f, 0, 5.0f);
			DrawDebugSphere(World, IconLocation, 25.0f, 12, FColor::Yellow, true, 0.1f);
			
			// Draw a pulsing effect
			DrawDebugSphere(World, IconLocation, 40.0f, 8, FColor::Orange, true, 0.1f);
			
			// Print pickup instruction
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, TEXT("🖐️ Press E to Pick Up!"));
			}
		}
		
		UE_LOG(LogTemp, Warning, TEXT("PICKUP ICON SHOWN for cube at: %s"), *GetActorLocation().ToString());
	}
}

void AHideAndSeekCube::HidePickupIcon()
{
	if (bPickupIconVisible)
	{
		bPickupIconVisible = false;
		UE_LOG(LogTemp, Log, TEXT("Pickup icon hidden"));
	}
}

void AHideAndSeekCube::UpdatePickupIcon()
{
	if (bIsPickedUp)
	{
		return; // Don't show icon if already picked up
	}

	// Check if player is nearby
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	// Get all overlapping actors
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, AActor::StaticClass());

	bool bPlayerNearby = false;
	for (AActor* Actor : OverlappingActors)
	{
		ABdozawaCharacter* PlayerCharacter = Cast<ABdozawaCharacter>(Actor);
		if (PlayerCharacter)
		{
			bPlayerNearby = true;
			break;
		}
	}

	// Show or hide icon based on player proximity
	if (bPlayerNearby && !bPickupIconVisible)
	{
		ShowPickupIcon();
	}
	else if (!bPlayerNearby && bPickupIconVisible)
	{
		HidePickupIcon();
	}
}
