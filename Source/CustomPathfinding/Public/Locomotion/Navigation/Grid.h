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
	FVector Center;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bObstacle;
};

USTRUCT(Blueprintable)
struct FCellArray 
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FCell> CellArray;

	FCell operator[] (int32 i) {
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
	// Sets default values for this actor's properties
	AGrid();

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 GridSizeX = 5;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 GridSizeY = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float NodeSize = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FCellArray> Cells;

};
