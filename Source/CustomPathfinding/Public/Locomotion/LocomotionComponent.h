// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "LocomotionComponent.generated.h"


class APath;
class UPathPlanner;

UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class CUSTOMPATHFINDING_API ULocomotionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	ULocomotionComponent();

	// Get controlled character
	const ACharacter* GetCharacter() const;

	// Set Goal Location turns on pathfinding
	void SetGoalLocation(FVector GoalLocation);

	// Get calculated move direction by steerings
	const FVector GetMoveDirection() const;

	// Utility for path planning
	bool CheckCharacterCanWalk(FVector StartLocation, FVector EndLocation);

	UFUNCTION(BlueprintCallable)
	void SetSmoothing(bool NewSmoothing);

public:

	// Waypoint distance acceptance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locomotion")
	double WaypointDistanceSq = 50.f;

	// Path smoothing optimization
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locomotion")
	bool bSmoothing = true;

	// Debug flag
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Locomotion")
	bool bDebug = true;

protected:

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	// Steering calculation (only path following)
	FVector Calculate();

private:

	// Sets path to follow
	void SetPath(APath* InPath);

	// Seeks target direction
	FVector Seek(FVector TargetLocation);

	// Path following
	FVector FollowPath();

private:

	UPROPERTY()
	UPathPlanner* m_pPathPlanner = nullptr;

	UPROPERTY()
	APath* m_pPath = nullptr;

	UPROPERTY(replicated)
	FVector m_MoveDirection = FVector::ZeroVector;

};

