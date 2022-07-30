// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomPathfindingPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "CustomPathfindingCharacter.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(LogCustomPathfindingPlayerController, Log, All);

ACustomPathfindingPlayerController::ACustomPathfindingPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	LocomotionComponent = CreateDefaultSubobject<ULocomotionComponent>(TEXT("LocomotionComponent"));
}

ULocomotionComponent* ACustomPathfindingPlayerController::GetLocomotionComponent() const
{
	return LocomotionComponent;
}

void ACustomPathfindingPlayerController::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ACustomPathfindingPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (APawn* pPawn = GetPawn())
	{
		// Use character movement for motion and replication
		pPawn->AddMovementInput(LocomotionComponent->GetMoveDirection(), 1.f, false);
	}
}

void ACustomPathfindingPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ACustomPathfindingPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ACustomPathfindingPlayerController::OnSetDestinationReleased);
}

void ACustomPathfindingPlayerController::OnSetDestinationPressed()
{
	// Just in case the character was moving because of a previous short press we stop it
	// StopMovement();	// this is using the navsystem
}

void ACustomPathfindingPlayerController::OnSetDestinationReleased()
{
	// We look for the location in the world where the player has pressed the input
	FVector HitLocation = FVector::ZeroVector;
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, true, Hit);
	HitLocation = Hit.Location;
	
	// VFXs
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, HitLocation, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);

	// Server RPC
	ServerSetDestination(Hit.Location);
}

void ACustomPathfindingPlayerController::ServerSetDestination_Implementation(FVector TargetLocation)
{
	if (HasAuthority())
	{
		LocomotionComponent->SetGoalLocation(TargetLocation);
	}
}
