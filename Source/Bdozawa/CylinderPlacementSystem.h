// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CylinderPlacementSystem.generated.h"

class ACylinderPickup;

/**
 * System to randomly place cylinder pickup objects in the playground
 */
UCLASS()
class BDOZAWA_API ACylinderPlacementSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	/** Constructor */
	ACylinderPlacementSystem();

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Number of cylinders to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
	int32 NumberOfCylinders = 8;

	/** Minimum distance from spawn point */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
	float MinDistance = 500.0f;

	/** Maximum distance from spawn point */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
	float MaxDistance = 2000.0f;

	/** Minimum height above ground */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
	float MinHeight = 50.0f;

	/** Maximum height above ground */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
	float MaxHeight = 300.0f;

	/** Cylinder pickup class to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Placement")
	TSubclassOf<ACylinderPickup> CylinderPickupClass;

	/** Reference to spawned cylinders */
	UPROPERTY(BlueprintReadOnly, Category = "Placement")
	TArray<ACylinderPickup*> SpawnedCylinders;

public:	
	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Place cylinders randomly in the playground */
	UFUNCTION(BlueprintCallable, Category = "Placement")
	void PlaceCylindersRandomly();

	/** Clear all spawned cylinders */
	UFUNCTION(BlueprintCallable, Category = "Placement")
	void ClearAllCylinders();

	/** Get all spawned cylinders */
	UFUNCTION(BlueprintPure, Category = "Placement")
	TArray<ACylinderPickup*> GetSpawnedCylinders() const { return SpawnedCylinders; }

	/** Get number of remaining (unpicked) cylinders */
	UFUNCTION(BlueprintPure, Category = "Placement")
	int32 GetRemainingCylinders() const;

	/** Get total number of cylinders that were spawned */
	UFUNCTION(BlueprintPure, Category = "Placement")
	int32 GetTotalCylinders() const { return SpawnedCylinders.Num(); }

private:
	/** Generate a random location for cylinder placement */
	FVector GenerateRandomLocation();

	/** Check if location is valid (not overlapping with existing objects) */
	bool IsLocationValid(const FVector& Location);

	/** Spawn a single cylinder at specified location */
	ACylinderPickup* SpawnCylinderAtLocation(const FVector& Location, const FString& CylinderName);
};
