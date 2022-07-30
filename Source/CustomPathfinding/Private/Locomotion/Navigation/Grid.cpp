// Fill out your copyright notice in the Description page of Project Settings.


#include "Locomotion/Navigation/Grid.h"

// Sets default values
AGrid::AGrid()
{
	PrimaryActorTick.bCanEverTick = false;
}

bool AGrid::WorldLocationToCell(FVector Location, FCell& OutCell)
{
	int32 row = FMath::TruncToInt32(Location.X / NodeSize);
	int32 column = FMath::TruncToInt32(Location.Y / NodeSize);

	if (row >= 0 && row < GridSizeX && column >= 0 && column < GridSizeY)
	{
		OutCell = Cells[row][column];
		return true;
	}
	return false;
}

void AGrid::GetNeighbours(FCell& Cell, TArray<FCell*>& OutNeighbours)
{
	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			// self node
			if (x == 0 && y == 0) 
			{ 
				continue; 
			}

			int checkX = Cell.Row + x;
			int checkY = Cell.Column + y;

			// is inside of the grid?
			if (checkX >= 0 && checkX < GridSizeX && checkY >= 0 && checkY < GridSizeY)
			{
				OutNeighbours.Add(&Cells[checkX][checkY]);
			}
		}
	}
}
