// Copyright Epic Games, Inc. All Rights Reserved.

#include "CylinderPickup.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/Engine.h"
#include "BdozawaCharacter.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

ACylinderPickup::ACylinderPickup()
{
	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	// Create the static mesh component
	CylinderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CylinderMesh"));
	RootComponent = CylinderMesh;
	
	// Make the cylinder more visible
	CylinderMesh->SetRelativeScale3D(FVector(1.5f, 1.5f, 2.0f));

	// Set default cylinder mesh if available
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMeshAsset(TEXT("/Engine/BasicShapes/Cylinder"));
	if (CylinderMeshAsset.Succeeded())
	{
		CylinderMesh->SetStaticMesh(CylinderMeshAsset.Object);
	}

	// Create the pickup sphere component
	PickupSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickupSphere"));
	PickupSphere->SetupAttachment(RootComponent);
	PickupSphere->SetSphereRadius(PickupRange);
	PickupSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickupSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	PickupSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// Initialize variables
	bIsPickedUp = false;
	CylinderName = TEXT("Unknown Cylinder");
}

void ACylinderPickup::BeginPlay()
{
	Super::BeginPlay();
	
	// Bind overlap events
	PickupSphere->OnComponentBeginOverlap.AddDynamic(this, &ACylinderPickup::OnOverlapBegin);
	PickupSphere->OnComponentEndOverlap.AddDynamic(this, &ACylinderPickup::OnOverlapEnd);
	
	// Start a timer to continuously show pickup icon when near player
	GetWorldTimerManager().SetTimer(PickupIconTimer, this, &ACylinderPickup::UpdatePickupIcon, 0.1f, true);
}

void ACylinderPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACylinderPickup::PickupCylinder()
{
	if (!bIsPickedUp)
	{
		bIsPickedUp = true;
		
		// Hide the cylinder completely
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);
		
		// Hide pickup icon
		HidePickupIcon();
		
		// Call Blueprint event
		OnCylinderPickedUp();
		
		// Print success message with cylinder name
		FString PickupMessage = FString::Printf(TEXT("🎯 %s picked up!"), *CylinderName);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, PickupMessage);
		}
		
		// Log to console
		UE_LOG(LogTemp, Warning, TEXT("%s PICKED UP at location: %s"), *CylinderName, *GetActorLocation().ToString());
		
	}
}

void ACylinderPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if the overlapping actor is the player character
	ABdozawaCharacter* PlayerCharacter = Cast<ABdozawaCharacter>(OtherActor);
	if (PlayerCharacter && !bIsPickedUp)
	{
		// Show pickup icon
		ShowPickupIcon();
		
		// Call Blueprint event for player entering range
		OnPlayerEnterRange();
		
		// Auto-pickup when player gets close (like HideAndSeekCube)
		PickupCylinder();
		
		UE_LOG(LogTemp, Warning, TEXT("Player entered pickup range for %s - auto-pickup"), *CylinderName);
	}
}

void ACylinderPickup::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Check if the leaving actor is the player character
	ABdozawaCharacter* PlayerCharacter = Cast<ABdozawaCharacter>(OtherActor);
	if (PlayerCharacter && !bIsPickedUp)
	{
		// Hide pickup icon
		HidePickupIcon();
		
		// Call Blueprint event for player leaving range
		OnPlayerLeaveRange();
		
		UE_LOG(LogTemp, Warning, TEXT("Player left pickup range for %s - hiding icon"), *CylinderName);
	}
}

void ACylinderPickup::ShowPickupIcon()
{
	if (!bPickupIconVisible && !bIsPickedUp)
	{
		bPickupIconVisible = true;
		
		// Draw a pickup icon above the cylinder
		if (UWorld* World = GetWorld())
		{
			// Draw a hand-like shape using debug lines
			FVector CylinderLocation = GetActorLocation();
			FVector IconLocation = CylinderLocation + FVector(0, 0, 200); // Above the cylinder
			
			// Draw hand icon (simple representation) - make it more visible
			DrawDebugLine(World, IconLocation + FVector(-30, 0, 0), IconLocation + FVector(30, 0, 0), FColor::Yellow, true, 0.1f, 0, 5.0f);
			DrawDebugLine(World, IconLocation + FVector(0, -30, 0), IconLocation + FVector(0, 30, 0), FColor::Yellow, true, 0.1f, 0, 5.0f);
			DrawDebugSphere(World, IconLocation, 25.0f, 12, FColor::Yellow, true, 0.1f);
			
			// Draw a pulsing effect
			DrawDebugSphere(World, IconLocation, 40.0f, 8, FColor::Orange, true, 0.1f);
			
			// Print pickup instruction with cylinder name
			FString PickupInstruction = FString::Printf(TEXT("🖐️ Press E to Pick Up %s!"), *CylinderName);
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Yellow, PickupInstruction);
			}
		}
		
		UE_LOG(LogTemp, Warning, TEXT("PICKUP ICON SHOWN for %s at: %s"), *CylinderName, *GetActorLocation().ToString());
	}
}

void ACylinderPickup::HidePickupIcon()
{
	if (bPickupIconVisible)
	{
		bPickupIconVisible = false;
		UE_LOG(LogTemp, Log, TEXT("Pickup icon hidden for %s"), *CylinderName);
	}
}

void ACylinderPickup::UpdatePickupIcon()
{
	// This function can be used to update the pickup icon visibility
	// For now, it's handled by the overlap events
}
