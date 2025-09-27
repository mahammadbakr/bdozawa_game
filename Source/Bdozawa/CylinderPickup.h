// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "CylinderPickup.generated.h"

/**
 * Cylinder pickup object that can be picked up by the player
 */
UCLASS()
class BDOZAWA_API ACylinderPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	/** Constructor */
	ACylinderPickup();

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Static mesh component for the cylinder */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* CylinderMesh;

	/** Collision sphere for pickup detection */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* PickupSphere;

	/** Whether this cylinder has been picked up */
	UPROPERTY(BlueprintReadOnly, Category = "Pickup")
	bool bIsPickedUp;

	/** Pickup range radius */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	float PickupRange = 150.0f;

public:	
	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Function to handle pickup */
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void PickupCylinder();

	/** Function to check if cylinder is picked up */
	UFUNCTION(BlueprintPure, Category = "Pickup")
	bool IsPickedUp() const { return bIsPickedUp; }

	/** Custom overlap function with correct signature */
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Overlap end function */
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** Blueprint event for when cylinder is picked up */
	UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
	void OnCylinderPickedUp();

	/** Blueprint event for when player enters pickup range */
	UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
	void OnPlayerEnterRange();

	/** Blueprint event for when player leaves pickup range */
	UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
	void OnPlayerLeaveRange();

	/** Show pickup icon */
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void ShowPickupIcon();

	/** Hide pickup icon */
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void HidePickupIcon();

	/** Whether pickup icon is visible */
	UPROPERTY(BlueprintReadOnly, Category = "Pickup")
	bool bPickupIconVisible = false;

	/** Timer for updating pickup icon */
	FTimerHandle PickupIconTimer;

	/** Update pickup icon visibility */
	void UpdatePickupIcon();

	/** Set the cylinder name for identification */
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void SetCylinderName(const FString& Name) { CylinderName = Name; }

	/** Get the cylinder name */
	UFUNCTION(BlueprintPure, Category = "Pickup")
	FString GetCylinderName() const { return CylinderName; }

private:
	/** Name of this cylinder for identification */
	FString CylinderName;
};
