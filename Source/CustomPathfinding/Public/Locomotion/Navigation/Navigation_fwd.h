// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Navigation_fwd.generated.h"


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

USTRUCT()
struct FGraphEdge
{
	GENERATED_BODY()

public:

	FGraphEdge(): 
		FGraphEdge(FVector::ZeroVector, FVector::ZeroVector)
	{
	}

	FGraphEdge(const FVector& InSource, const FVector& InDestination): 
		Source(InSource), Destination(InDestination)
	{
	}

	FVector Source;
	FVector Destination;
};
