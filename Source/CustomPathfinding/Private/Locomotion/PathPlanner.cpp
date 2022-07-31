// Fill out your copyright notice in the Description page of Project Settings.


#include "Locomotion/PathPlanner.h"

#include "Locomotion/LocomotionComponent.h"
#include "Locomotion/Navigation/Grid.h"

DEFINE_LOG_CATEGORY_STATIC(LogPathPlanner, Log, All);

void UPathPlanner::Init(AGrid* InGrid)
{
	m_pGrid = InGrid;
}

bool UPathPlanner::CreatePathToLocation(FVector StartLocation, FVector TargetLocation)
{
	m_Path.Empty();

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
	//AStar search(m_pGrid, startNode, targetNode);
	
	TArray<FCell*> openSet;
	TArray<FCell*> closedSet;
	
	openSet.Add(&startNode);
	while (openSet.Num() > 0)
	{
		FCell* currentNode = openSet[0];
		for (int i = 1; i < openSet.Num(); i++)
		{
			if (openSet[i]->fCost() < currentNode->fCost()
				|| openSet[i]->fCost() == currentNode->fCost() && openSet[i]->hCost < currentNode->hCost)
			{
				currentNode = openSet[i];
			}
		}

		openSet.Remove(currentNode);
		closedSet.Add(currentNode);

		// path has been found
		if (*currentNode == targetNode)
		{
			RetracePath(startNode, *currentNode);

			// Smooth path post-process
			SmoothPath(StartLocation, TargetLocation);

			return true;
		}

		TArray<FCell*> neighbours;
		m_pGrid->GetNeighbours(*currentNode, neighbours);
		for (auto It = neighbours.CreateIterator(); It; ++It)
		{
			FCell* neighbour = *It;
			// discard nodes if not traversable or in closed set
			if (neighbour->bObstacle || closedSet.Contains(neighbour))
			{
				continue;
			}

			int32 newMovementCostToNeighbour = currentNode->gCost + GetDistance(*currentNode, *neighbour);
			if (newMovementCostToNeighbour < neighbour->gCost || !openSet.Contains(neighbour))
			{
				// update costs
				neighbour->gCost = newMovementCostToNeighbour;
				neighbour->hCost = GetDistance(*neighbour, targetNode);
				// set the parent
				neighbour->ParentRow = currentNode->Row;
				neighbour->ParentColumn = currentNode->Column;

				if( !openSet.Contains(neighbour) )
				{
					openSet.Add(neighbour);
				}
			}
		}
	}

	// 4. Grab the path
	//OutPath = search.GetPath();

	return false;
}

AGrid* UPathPlanner::GetGrid() const
{
	return m_pGrid;
}

const TArray<FCell> UPathPlanner::GetPath()
{
	return m_Path;
}

const TArray<FVector> UPathPlanner::GetLocationPath() const
{
	return m_LocationPath;
}

int UPathPlanner::GetDistance(const FCell& NodeA, const FCell& NodeB)
{
	int32 dstX = FMath::Abs(NodeA.Row - NodeB.Row);
	int32 dstY = FMath::Abs(NodeA.Column - NodeB.Column);

	if (dstX > dstY)
	{
		return 14 * dstY + 10 * (dstX - dstY);
	}
	return 14 * dstX + 10 * (dstY - dstX);
}

void UPathPlanner::RetracePath(FCell& StartNode, FCell& TargetNode)
{
	FCell currentNode = TargetNode;
	while ( currentNode != StartNode )
	{
		m_Path.Add(currentNode);
		currentNode = m_pGrid->Cells[currentNode.ParentRow][currentNode.ParentColumn];
	}
	m_Path.Add(currentNode);
	Algo::Reverse(m_Path);
}

void UPathPlanner::SmoothPath(const  FVector& StartLocation, const  FVector& TargetLocation)
{
	// Convert Nodes path to Edges path
	std::list<FGraphEdge> pathEdges;
	pathEdges.push_back(FGraphEdge(StartLocation, m_Path[0].Center));
	for (int i = 0; i < m_Path.Num()-1; i++)
	{
		pathEdges.push_back(FGraphEdge(m_Path[i].Center, m_Path[i+1].Center));
	}
	pathEdges.push_back(FGraphEdge(m_Path.Last().Center, TargetLocation));

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

	// Construct the new path
	m_LocationPath.Empty();
	for (auto it = pathEdges.begin(); it != pathEdges.end(); ++it)
	{
		m_LocationPath.Add(it->Destination);
	}

}
