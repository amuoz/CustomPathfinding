// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "Navigation_fwd.h"

#include "Grid.generated.h"

/*
* Navigable world space represented as a Grid of Cells.
* Needed for path planning.
*/
UCLASS()
class CUSTOMPATHFINDING_API AGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AGrid();

	// Location to grid Cell
	bool WorldLocationToCell(FVector Location, FCell& OutCell);

	// Get cell grid neighbours in all 8 directions
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
