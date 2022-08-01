// Fill out your copyright notice in the Description page of Project Settings.


#include "Locomotion/Navigation/PathPlanner.h"

#include "Locomotion/LocomotionComponent.h"
#include "Locomotion/Navigation/Grid.h"
#include "Locomotion/Navigation/SearchAStar.h"

DEFINE_LOG_CATEGORY_STATIC(LogPathPlanner, Log, All);

void UPathPlanner::Init(AGrid* InGrid, bool InbSmoothing)
{
	m_pGrid = InGrid;
	bSmoothing = InbSmoothing;
}

bool UPathPlanner::CreatePathToLocation(FVector StartLocation, FVector TargetLocation, TArray<FVector>& OutLocationPath)
{
	// 1. Get closest node to start location
	FCell startNode;
	if (!m_pGrid->WorldLocationToCell(StartLocation, startNode))
	{
		return false;
	}

	// 2. Get closest node to goal location
	FCell targetNode;
	if (!m_pGrid->WorldLocationToCell(TargetLocation, targetNode))
	{
		return false;
	}

	// 3. AStar search
	SearchAStar search(*m_pGrid, startNode, targetNode);
	
	// 4. Grab the path
	TArray<FCell> outPath = search.GetPath();
	if (!outPath.IsEmpty())
	{
		// Convert cells into location
		CellToLocationPath(outPath, OutLocationPath);

		if (bSmoothing)
		{
			// Path smoothing post-process
			SmoothPath(StartLocation, TargetLocation, OutLocationPath);
		}
		else
		{
			// Add the target position to the end
			OutLocationPath.Add(TargetLocation);
		}

		return true;
	}

	return false;
}

AGrid* UPathPlanner::GetGrid() const
{
	return m_pGrid;
}

void UPathPlanner::SetSmoothing(bool NewSmoothing)
{
	bSmoothing = NewSmoothing;
}

void UPathPlanner::CellToLocationPath(const TArray<FCell>& CellPath, TArray<FVector>& LocationPath)
{
	LocationPath.Empty();
	for (auto It = CellPath.CreateConstIterator(); It; ++It)
	{
		LocationPath.Add(It->Center);
	}
}

void UPathPlanner::SmoothPath(const  FVector& StartLocation, const  FVector& TargetLocation, TArray<FVector>& LocationPath)
{
	// 1. Convert path to edge path
	std::list<FGraphEdge> pathEdges;
	pathEdges.push_back(FGraphEdge(StartLocation, LocationPath[0]));
	for (int i = 0; i < LocationPath.Num()-1; i++)
	{
		pathEdges.push_back(FGraphEdge(LocationPath[i], LocationPath[i+1]));
	}
	pathEdges.push_back(FGraphEdge(LocationPath.Last(), TargetLocation));

	// 2. Remove unneccesary edges
	auto it1 = pathEdges.begin();
	auto it2 = pathEdges.begin();

	// increment it2 to the node following it1
	++it2;
	// while it2 is not the last node in the path, step through the nodes
	while (it2 != pathEdges.end())
	{
		// check for obstruction and adjust accordingly
		ULocomotionComponent* pLocomotion = Cast<ULocomotionComponent>(GetOuter());
		if (pLocomotion->CheckCharacterCanWalk(it1->Source, it2->Destination))
		{
			it1->Destination = it2->Destination;
			it2 = pathEdges.erase(it2);
		}
		else
		{
			it1 = it2;
			++it2;
		}
	}

	// 3. Construct new location path
	LocationPath.Empty();
	for (auto it = pathEdges.begin(); it != pathEdges.end(); ++it)
	{
		LocationPath.Add(it->Destination);
	}
}
