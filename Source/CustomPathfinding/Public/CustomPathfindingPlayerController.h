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

	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	UFUNCTION(Server, Reliable)
	void ServerSetDestination(FVector TargetLocation);

	UFUNCTION()
	void OnRep_GoalLocation();

private:

	UPROPERTY(ReplicatedUsing = OnRep_GoalLocation)
	FVector GoalLocation = FVector::ZeroVector;

	UPROPERTY()
	ULocomotionComponent* LocomotionComponent = nullptr;

};
