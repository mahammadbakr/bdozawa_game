// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BdozawaGameMode.generated.h"

class AHideAndSeekCube;

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
};



