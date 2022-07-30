// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Grid.generated.h"


USTRUCT(Blueprintable)
struct FCell
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Row;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 Column;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector Center;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bObstacle;

	int32 gCost;

	int32 hCost;

	int ParentRow;

	int ParentColumn;

public:

	// fCost = gCost + hCost
	int32 fCost() 
	{ 
		return gCost + hCost; 
	}

	inline bool operator==(const FCell& OtherCell) const
	{
		return Row == OtherCell.Row 
			&& Column == OtherCell.Column;
	}

	inline bool operator!= (const FCell& OtherCell) const
	{
		return Row != OtherCell.Row
			|| Column != OtherCell.Column;
	}
};

USTRUCT(Blueprintable)
struct FCellArray 
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FCell> CellArray;

	FCell& operator[] (int32 i) {
		return CellArray[i];
	}

	void Add(FCell Cell) {
		CellArray.Add(Cell);
	}
};

UCLASS()
class CUSTOMPATHFINDING_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AGrid();

	// Location to Cell
	bool WorldLocationToCell(FVector Location, FCell& OutCell);

	void GetNeighbours(FCell& Cell, TArray<FCell*>& Neighbours);

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 GridSizeX = 5;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 GridSizeY = 5;

	// Not editable for cube static mesh size convenience
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float NodeSize = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FCellArray> Cells;

};
