// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "Locomotion/LocomotionComponent.h"

#include "CustomPathfindingPlayerController.generated.h"


/** Forward declaration to improve compiling times */
class UNiagaraSystem;

UCLASS()
class ACustomPathfindingPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ACustomPathfindingPlayerController();

	// Get agent locomotion component
	UFUNCTION(BlueprintCallable)
	ULocomotionComponent* GetLocomotionComponent() const;

public:

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Input handlers for SetDestination action. */
	void OnSetDestinationReleased();

	// Sends clicked goal location to server
	UFUNCTION(Server, Reliable)
	void ServerSetDestination(FVector TargetLocation);

private:

	UPROPERTY(EditDefaultsOnly)
	ULocomotionComponent* LocomotionComponent = nullptr;

};
