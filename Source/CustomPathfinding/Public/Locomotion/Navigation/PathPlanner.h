// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Locomotion/Navigation/Navigation_fwd.h"

#include "PathPlanner.generated.h"

class AGrid;

/**
 * Path planner get shortest path possible using AStar search.
 * Optional path smoothing allowed.
 */
UCLASS()
class CUSTOMPATHFINDING_API UPathPlanner : public UObject
{
	GENERATED_BODY()

public:

	// Initialize path planner with navigable grid instance
	void Init(AGrid* InGrid, bool InbSmoothing);

	// Finds least cost path from character location to target location.
	// Fills path with a list of locations
	// Returns true if searh is successful, false otherwise
	bool CreatePathToLocation(FVector StartLocation, FVector TargetLocation, TArray<FVector>& LocationPath);

	// Gets grid actor
	AGrid* GetGrid() const;

	void SetSmoothing(bool NewSmoothing);

private:

	// Convert list of nodes to vector locations
	void CellToLocationPath(const TArray<FCell>& CellPath, TArray<FVector>& OutLocationPath);

	// Smoothing post-processing for more human like paths
	void SmoothPath(const  FVector& StartLocation, const  FVector& TargetLocation, TArray<FVector>& LocationPath);

private:

	UPROPERTY()
	AGrid* m_pGrid;

	bool bSmoothing;
};
