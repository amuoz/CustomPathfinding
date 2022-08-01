// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Grid.h"

/**
 * Search AStar algorithm.
 * It makes a Search upon construction.
 * Method GetPath for getting the result Path.
 *
 */
class CUSTOMPATHFINDING_API SearchAStar
{
public:

	SearchAStar(AGrid& Grid, FCell& Source, FCell& Target);
	~SearchAStar();

	// List of nodes of the path
	TArray<FCell> GetPath() const;

private:

	// A* main method
	void Search();

	// Manhattan heuristic
	int GetDistance(const FCell& NodeA, const FCell& NodeB);

	// Retrace Nodes using parent pointer to conform the path
	void RetracePath(FCell& StartNode, FCell& TargetNode);

private:

	AGrid& m_Grid;

	FCell m_Source;

	FCell m_Target;

	TArray<FCell> m_Path;
};
