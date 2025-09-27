// Copyright Epic Games, Inc. All Rights Reserved.

#include "BdozawaGameMode.h"
#include "HideAndSeekCube.h"
#include "CylinderPlacementSystem.h"
#include "BdozawaCharacter.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"

ABdozawaGameMode::ABdozawaGameMode()
{
	// Set default cube class
	CubeClass = AHideAndSeekCube::StaticClass();
	
	// Set default cylinder placement system class
	CylinderPlacementSystemClass = ACylinderPlacementSystem::StaticClass();
	CylinderPlacementSystem = nullptr;
	
	// Set default pawn class to use third person character
	DefaultPawnClass = ABdozawaCharacter::StaticClass();
}

void ABdozawaGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("ABdozawaGameMode::BeginPlay() called!"));
	
	// Spawn the hide and seek cubes
	SpawnHideAndSeekCubes();
	
	// Spawn the cylinder placement system (this will handle cylinder spawning)
	SpawnCylinderPlacementSystem();
	
	// Spawn hidden cylinder objects (legacy system)
	SpawnHiddenObjects();
}

void ABdozawaGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UE_LOG(LogTemp, Warning, TEXT("ABdozawaGameMode::PostInitializeComponents() called!"));
}

void ABdozawaGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	UE_LOG(LogTemp, Warning, TEXT("ABdozawaGameMode::InitGame() called!"));
	
	// Spawn the hide and seek cubes
	SpawnHideAndSeekCubes();
	
	// Spawn the cylinder placement system (this will handle cylinder spawning)
	SpawnCylinderPlacementSystem();
	
	// Spawn hidden cylinder objects (legacy system)
	SpawnHiddenObjects();
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

void ABdozawaGameMode::SpawnHiddenObjects()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("World is null!"));
		return;
	}

	// List of cylinder object names to spawn
	TArray<FString> CylinderNames = {
		TEXT("SM_Cylinder2"),
		TEXT("SM_Cylinder3"),
		TEXT("SM_Cylinder4"),
		TEXT("SM_Cylinder5"),
		TEXT("SM_Cylinder6"),
		TEXT("SM_Cylinder7"),
		TEXT("SM_Cylinder8"),
		TEXT("SM_Cylinder9")
	};

	// Clear existing hidden objects
	for (AActor* HiddenObject : HiddenObjects)
	{
		if (IsValid(HiddenObject))
		{
			HiddenObject->Destroy();
		}
	}
	HiddenObjects.Empty();

	// Spawn each cylinder at random locations
	for (int32 i = 0; i < CylinderNames.Num(); i++)
	{
		// Generate random position within spawn area
		FVector RandomLocation;
		RandomLocation.X = FMath::RandRange(SpawnAreaMin.X, SpawnAreaMax.X);
		RandomLocation.Y = FMath::RandRange(SpawnAreaMin.Y, SpawnAreaMax.Y);
		RandomLocation.Z = FMath::RandRange(50.0f, 200.0f); // Spawn on ground level

		// Create a simple static mesh actor
		AActor* NewHiddenObject = World->SpawnActor<AActor>(AActor::StaticClass(), RandomLocation, FRotator::ZeroRotator);
		
		if (NewHiddenObject)
		{
			// Add a static mesh component
			UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(NewHiddenObject);
			MeshComponent->SetupAttachment(NewHiddenObject->GetRootComponent());
			
			// Try to load the cylinder mesh
			UStaticMesh* CylinderMesh = LoadObject<UStaticMesh>(nullptr, *FString::Printf(TEXT("/Game/%s"), *CylinderNames[i]));
			if (CylinderMesh)
			{
				MeshComponent->SetStaticMesh(CylinderMesh);
			}
			else
			{
				// Fallback to basic cylinder if specific mesh not found
				CylinderMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder"));
				if (CylinderMesh)
				{
					MeshComponent->SetStaticMesh(CylinderMesh);
				}
			}

			// Make the object visible but blend with environment
			MeshComponent->SetRelativeScale3D(FVector(1.0f, 1.0f, 2.0f)); // Make cylinders taller
			MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			
			HiddenObjects.Add(NewHiddenObject);
			
			UE_LOG(LogTemp, Warning, TEXT("Spawned hidden object %s at location: %s"), *CylinderNames[i], *RandomLocation.ToString());
		}
	}

	// Print debug message
	if (GEngine)
	{
		FString Message = FString::Printf(TEXT("Spawned %d hidden cylinder objects!"), HiddenObjects.Num());
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, Message);
	}
}

void ABdozawaGameMode::SpawnCylinderPlacementSystem()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("World is null in SpawnCylinderPlacementSystem!"));
		return;
	}

	if (!CylinderPlacementSystemClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("CylinderPlacementSystemClass is not set!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Spawning Cylinder Placement System..."));

	// Spawn the cylinder placement system at the origin
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	CylinderPlacementSystem = World->SpawnActor<ACylinderPlacementSystem>(CylinderPlacementSystemClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	
	if (CylinderPlacementSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Successfully spawned Cylinder Placement System!"));
		
		// Print debug message
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("🎯 Cylinder Placement System spawned!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn Cylinder Placement System!"));
	}
}
