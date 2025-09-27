// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BdozawaGameMode.generated.h"

class AHideAndSeekCube;
class ACylinderPlacementSystem;

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class BDOZAWA_API ABdozawaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	ABdozawaGameMode();

protected:
	/** Called when the game starts */
	virtual void BeginPlay() override;

	/** Called after components are initialized */
	virtual void PostInitializeComponents() override;

	/** Called when game is initialized */
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	/** Number of cubes to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HideAndSeek")
	int32 NumberOfCubes = 2;

	/** Class of the cube to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HideAndSeek")
	TSubclassOf<AHideAndSeekCube> CubeClass;

	/** Spawn area bounds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HideAndSeek")
	FVector SpawnAreaMin = FVector(-1000, -1000, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HideAndSeek")
	FVector SpawnAreaMax = FVector(1000, 1000, 0);

	/** Spawn the hide and seek cubes */
	UFUNCTION(BlueprintCallable, Category = "HideAndSeek")
	void SpawnHideAndSeekCubes();

	/** Array to store spawned cubes */
	UPROPERTY(BlueprintReadOnly, Category = "HideAndSeek")
	TArray<AHideAndSeekCube*> SpawnedCubes;

	/** Array to store hidden cylinder objects */
	UPROPERTY(BlueprintReadOnly, Category = "HideAndSeek")
	TArray<AActor*> HiddenObjects;

	/** Spawn hidden cylinder objects */
	UFUNCTION(BlueprintCallable, Category = "HideAndSeek")
	void SpawnHiddenObjects();

	/** Cylinder placement system class */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cylinders")
	TSubclassOf<ACylinderPlacementSystem> CylinderPlacementSystemClass;

	/** Reference to the cylinder placement system */
	UPROPERTY(BlueprintReadOnly, Category = "Cylinders")
	ACylinderPlacementSystem* CylinderPlacementSystem;

	/** Spawn cylinder placement system */
	UFUNCTION(BlueprintCallable, Category = "Cylinders")
	void SpawnCylinderPlacementSystem();
};



