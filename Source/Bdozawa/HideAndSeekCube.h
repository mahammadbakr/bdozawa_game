// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "HideAndSeekCube.generated.h"

/**
 * Hide and Seek Cube that can be picked up by the player
 */
UCLASS()
class BDOZAWA_API AHideAndSeekCube : public AActor
{
	GENERATED_BODY()
	
public:	
	/** Constructor */
	AHideAndSeekCube();

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Static mesh component for the cube */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* CubeMesh;

	/** Collision sphere for pickup detection */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* PickupSphere;

	/** Whether this cube has been picked up */
	UPROPERTY(BlueprintReadOnly, Category = "HideAndSeek")
	bool bIsPickedUp;

public:	
	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Function to handle pickup */
	UFUNCTION(BlueprintCallable, Category = "HideAndSeek")
	void PickupCube();

	/** Function to check if cube is picked up */
	UFUNCTION(BlueprintPure, Category = "HideAndSeek")
	bool IsPickedUp() const { return bIsPickedUp; }

	/** Override to handle overlap events */
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	/** Custom overlap function with correct signature */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Blueprint event for when cube is picked up */
	UFUNCTION(BlueprintImplementableEvent, Category = "HideAndSeek")
	void OnCubePickedUp();

	/** Blueprint event for when player enters pickup range */
	UFUNCTION(BlueprintImplementableEvent, Category = "HideAndSeek")
	void OnPlayerEnterRange();

	/** Blueprint event for when player leaves pickup range */
	UFUNCTION(BlueprintImplementableEvent, Category = "HideAndSeek")
	void OnPlayerLeaveRange();
};
