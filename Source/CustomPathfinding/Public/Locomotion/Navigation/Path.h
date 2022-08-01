// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Path.generated.h"

class ATargetPoint;

/*
* Path represents a list of waypoints to traverse.
* It allows to track the path and check if path has finished.
*/
UCLASS()
class CUSTOMPATHFINDING_API APath : public AActor
{
	GENERATED_BODY()
	
public:	

	APath();

	const FVector GetCurrentWaypoint() const;

	void SetNextWaypoint();

	bool IsFinished() const;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FVector> Waypoints;

private:	

	int32 m_CurrentWaypoint = 0;

};
