// Copyright Epic Games, Inc. All Rights Reserved.

#include "BdozawaGameMode.h"
#include "HideAndSeekCube.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"

ABdozawaGameMode::ABdozawaGameMode()
{
	// Set default cube class
	CubeClass = AHideAndSeekCube::StaticClass();
}

void ABdozawaGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	// Spawn the hide and seek cubes
	SpawnHideAndSeekCubes();
}

void ABdozawaGameMode::SpawnHideAndSeekCubes()
{
	UE_LOG(LogTemp, Warning, TEXT("SpawnHideAndSeekCubes called!"));
	
	if (!CubeClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("CubeClass is not set!"));
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("World is null!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("About to spawn %d cubes"), NumberOfCubes);

	// Clear existing cubes
	for (AHideAndSeekCube* Cube : SpawnedCubes)
	{
		if (IsValid(Cube))
		{
			Cube->Destroy();
		}
	}
	SpawnedCubes.Empty();

	// Spawn new cubes
	for (int32 i = 0; i < NumberOfCubes; i++)
	{
		// Generate random position within spawn area - spawn closer to origin for testing
		FVector RandomLocation;
		RandomLocation.X = FMath::RandRange(-500.0f, 500.0f);  // Smaller area around origin
		RandomLocation.Y = FMath::RandRange(-500.0f, 500.0f);
		RandomLocation.Z = 200.0f;  // Fixed height above ground

		// Spawn the cube
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		
		UE_LOG(LogTemp, Warning, TEXT("Attempting to spawn cube %d at location: %s"), i, *RandomLocation.ToString());
		
		AHideAndSeekCube* NewCube = World->SpawnActor<AHideAndSeekCube>(CubeClass, RandomLocation, FRotator::ZeroRotator, SpawnParams);
		
		if (NewCube)
		{
			SpawnedCubes.Add(NewCube);
			UE_LOG(LogTemp, Warning, TEXT("Successfully spawned cube %d at location: %s"), i, *RandomLocation.ToString());
			
			// Draw debug sphere to show where cube spawned
			DrawDebugSphere(World, RandomLocation, 100.0f, 12, FColor::Blue, true, 10.0f);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn cube %d at location: %s"), i, *RandomLocation.ToString());
		}
	}

	// Print debug message
	if (GEngine)
	{
		FString Message = FString::Printf(TEXT("Spawned %d hide and seek cubes!"), SpawnedCubes.Num());
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, Message);
	}
}
