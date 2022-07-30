// Fill out your copyright notice in the Description page of Project Settings.


#include "Locomotion/PathPlanner.h"

#include "Locomotion/LocomotionComponent.h"
#include "GameFramework/Character.h"
#include "Locomotion/Navigation/Grid.h"

DEFINE_LOG_CATEGORY_STATIC(LogPathPlanner, Log, All);

void UPathPlanner::Init(AGrid* InGrid)
{
	m_pGrid = InGrid;
}

bool UPathPlanner::CreatePathToLocation(FVector StartLocation, FVector TargetLocation)
{
	m_Path.Empty();

	// 1. Get closest node to character location
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

void UPathPlanner::RetracePath(FCell& startNode, FCell& targetNode)
{
	FCell currentNode = targetNode;
	while ( currentNode != startNode )
	{
		m_Path.Add(currentNode);
		currentNode = m_pGrid->Cells[currentNode.ParentRow][currentNode.ParentColumn];
	}
	Algo::Reverse(m_Path);
}
