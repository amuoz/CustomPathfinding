// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Locomotion/Navigation/Grid.h"

#include "PathPlanner.generated.h"

class APath;

/**
 * 
 */
UCLASS()
class CUSTOMPATHFINDING_API UPathPlanner : public UObject
{
	GENERATED_BODY()

public:

	// Initializa path planner with navigable grid instance
	void Init(AGrid* InGrid);

	// Finds least cost path from character location to target location.
	// Fills path with a list of waypoints
	// Returns true if searh is successful, false otherwise
	bool CreatePathToLocation(FVector StartLocation, FVector TargetLocation);

	AGrid* GetGrid() const;

	const TArray<FCell> GetPath();

private:

	int GetDistance(const FCell& NodeA, const FCell& NodeB);

	void RetracePath(FCell& startNode, FCell& targetNode);

private:

	UPROPERTY()
	AGrid* m_pGrid;

	UPROPERTY()
	TArray<FCell> m_Path;
};
