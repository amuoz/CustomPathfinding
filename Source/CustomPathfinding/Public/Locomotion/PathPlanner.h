// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PathPlanner.generated.h"

class APath;
class AGrid;

/**
 * 
 */
UCLASS()
class CUSTOMPATHFINDING_API UPathPlanner : public UObject
{
	GENERATED_BODY()

public:

	// Initializa path planner with navigable grid instance
	void Init(const AGrid* InGrid);

	// Finds least cost path from character location to target location.
	// Fills path with a list of waypoints
	// Returns true if searh is successful, false otherwise
	bool CreatePathToLocation(FVector InTargetLocation, APath& OutPath);

private:

	int GetClosestNodeToLocation(FVector InLocation);

private:

	UPROPERTY()
	const AGrid* m_pGrid;

	// Goal location to plan a path
	FVector m_TargetLocation;

};
