// Fill out your copyright notice in the Description page of Project Settings.


#include "Locomotion/Navigation/SearchAStar.h"

#include "Locomotion/Navigation/Grid.h"

SearchAStar::SearchAStar(AGrid& Grid, FCell& Source, FCell& Target):
	m_Grid(Grid), m_Source(Source), m_Target(Target)
{
    Search();
}

SearchAStar::~SearchAStar()
{

}

TArray<FCell> SearchAStar::GetPath() const
{
    return m_Path;
}

void SearchAStar::Search()
{
	TArray<FCell*> openSet;
	TArray<FCell*> closedSet;

	openSet.Add(&m_Source);
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
		if (*currentNode == m_Target)
		{
			RetracePath(m_Source, *currentNode);
			return;
		}

		TArray<FCell*> neighbours;
		m_Grid.GetNeighbours(*currentNode, neighbours);
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
				neighbour->hCost = GetDistance(*neighbour, m_Target);
				// set the parent
				neighbour->ParentRow = currentNode->Row;
				neighbour->ParentColumn = currentNode->Column;

				if (!openSet.Contains(neighbour))
				{
					openSet.Add(neighbour);
				}
			}
		}
	}
}

int SearchAStar::GetDistance(const FCell& NodeA, const FCell& NodeB)
{
	int32 dstX = FMath::Abs(NodeA.Row - NodeB.Row);
	int32 dstY = FMath::Abs(NodeA.Column - NodeB.Column);

	if (dstX > dstY)
	{
		return 14 * dstY + 10 * (dstX - dstY);
	}
	return 14 * dstX + 10 * (dstY - dstX);
}

void SearchAStar::RetracePath(FCell& StartNode, FCell& TargetNode)
{
	FCell currentNode = TargetNode;
	while (currentNode != StartNode)
	{
		m_Path.Add(currentNode);
		currentNode = m_Grid.Cells[currentNode.ParentRow][currentNode.ParentColumn];
	}
	// Add starting node
	m_Path.Add(currentNode);
	
	Algo::Reverse(m_Path);
}
