// Copyright Epic Games, Inc. All Rights Reserved.

#include "CylinderPlacementSystem.h"
#include "CylinderPickup.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

ACylinderPlacementSystem::ACylinderPlacementSystem()
{
	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	// Set default values for playground area
	NumberOfCylinders = 8;
	MinDistance = 800.0f;  // Start further from center
	MaxDistance = 3500.0f; // Use larger playground area
	MinHeight = 50.0f;
	MaxHeight = 300.0f;
}

void ACylinderPlacementSystem::BeginPlay()
{
	Super::BeginPlay();
	
	// Set the default cylinder pickup class if not set
	if (!CylinderPickupClass)
	{
		CylinderPickupClass = ACylinderPickup::StaticClass();
	}
	
	// Automatically place cylinders when the system starts
	PlaceCylindersRandomly();
}

void ACylinderPlacementSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACylinderPlacementSystem::PlaceCylindersRandomly()
{
	// Clear any existing cylinders first
	ClearAllCylinders();
	
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Error, TEXT("World is null in PlaceCylindersRandomly!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Placing %d cylinders randomly..."), NumberOfCylinders);

	int32 SuccessfullyPlaced = 0;
	int32 MaxAttempts = NumberOfCylinders * 10; // Prevent infinite loops
	int32 Attempts = 0;

	for (int32 i = 0; i < NumberOfCylinders && Attempts < MaxAttempts; ++i)
	{
		Attempts++;
		
		FVector RandomLocation = GenerateRandomLocation();
		
		if (IsLocationValid(RandomLocation))
		{
			FString CylinderName = FString::Printf(TEXT("SM_Cylinder%d"), i + 2); // Start from 2 to match SM_Cylinder2, SM_Cylinder3, etc.
			ACylinderPickup* NewCylinder = SpawnCylinderAtLocation(RandomLocation, CylinderName);
			
			if (NewCylinder)
			{
				SpawnedCylinders.Add(NewCylinder);
				SuccessfullyPlaced++;
				
				UE_LOG(LogTemp, Warning, TEXT("Successfully placed %s at: %s"), *CylinderName, *RandomLocation.ToString());
			}
		}
	}

	// Show placement results
	FString ResultMessage = FString::Printf(TEXT("🎯 Placed %d cylinders in the playground!"), SuccessfullyPlaced);
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, ResultMessage);
	}

	UE_LOG(LogTemp, Warning, TEXT("Cylinder placement complete: %d/%d cylinders placed"), SuccessfullyPlaced, NumberOfCylinders);
}

void ACylinderPlacementSystem::ClearAllCylinders()
{
	for (ACylinderPickup* Cylinder : SpawnedCylinders)
	{
		if (IsValid(Cylinder))
		{
			Cylinder->Destroy();
		}
	}
	SpawnedCylinders.Empty();
	
	UE_LOG(LogTemp, Warning, TEXT("Cleared all cylinders"));
}

FVector ACylinderPlacementSystem::GenerateRandomLocation()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return FVector::ZeroVector;
	}

	// Get random angle and distance
	float RandomAngle = FMath::FRandRange(0.0f, 360.0f);
	float RandomDistance = FMath::FRandRange(MinDistance, MaxDistance);
	float RandomHeight = FMath::FRandRange(MinHeight, MaxHeight);

	// Convert to world coordinates
	float X = FMath::Cos(FMath::DegreesToRadians(RandomAngle)) * RandomDistance;
	float Y = FMath::Sin(FMath::DegreesToRadians(RandomAngle)) * RandomDistance;
	float Z = RandomHeight;

	// Start from the system's location (or origin if not set)
	FVector BaseLocation = GetActorLocation();
	return BaseLocation + FVector(X, Y, Z);
}

bool ACylinderPlacementSystem::IsLocationValid(const FVector& Location)
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	// Check for overlaps with existing cylinders - increased minimum distance
	for (ACylinderPickup* ExistingCylinder : SpawnedCylinders)
	{
		if (IsValid(ExistingCylinder))
		{
			float Distance = FVector::Dist(Location, ExistingCylinder->GetActorLocation());
			if (Distance < 400.0f) // Increased minimum distance between cylinders
			{
				return false;
			}
		}
	}

	// You could add more validation here (e.g., ground check, obstacle check)
	return true;
}

ACylinderPickup* ACylinderPlacementSystem::SpawnCylinderAtLocation(const FVector& Location, const FString& CylinderName)
{
	UWorld* World = GetWorld();
	if (!World || !CylinderPickupClass)
	{
		return nullptr;
	}

	// Spawn the cylinder
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ACylinderPickup* NewCylinder = World->SpawnActor<ACylinderPickup>(CylinderPickupClass, Location, FRotator::ZeroRotator, SpawnParams);
	
	if (NewCylinder)
	{
		// Set the cylinder name
		NewCylinder->SetCylinderName(CylinderName);
		
		// Make sure it's visible and active
		NewCylinder->SetActorHiddenInGame(false);
		NewCylinder->SetActorEnableCollision(true);
		NewCylinder->SetActorTickEnabled(true);
	}

	return NewCylinder;
}

int32 ACylinderPlacementSystem::GetRemainingCylinders() const
{
	int32 Remaining = 0;
	for (ACylinderPickup* Cylinder : SpawnedCylinders)
	{
		if (IsValid(Cylinder) && !Cylinder->IsPickedUp())
		{
			Remaining++;
		}
	}
	return Remaining;
}
