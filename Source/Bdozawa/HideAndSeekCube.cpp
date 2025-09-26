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
		
		// Hide the cube
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		
		// Call Blueprint event
		OnCubePickedUp();
		
		// Print debug message
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Cube Picked Up!"));
		}
		
		// Draw debug sphere to show where cube was
		if (UWorld* World = GetWorld())
		{
			DrawDebugSphere(World, GetActorLocation(), 50.0f, 12, FColor::Red, true, 5.0f);
		}
	}
}

void AHideAndSeekCube::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
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
