// Fill out your copyright notice in the Description page of Project Settings.


#include "Locomotion/PathPlanner.h"

#include "Locomotion/LocomotionComponent.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(LogPathPlanner, Log, All);

void UPathPlanner::Init(const AGrid* InGrid)
{
	m_pGrid = InGrid;
}

bool UPathPlanner::CreatePathToLocation(FVector InTargetLocation, APath& OutPath)
{
	m_TargetLocation = InTargetLocation;

	// 1. Get closest node to character location
	ULocomotionComponent* pLocomotion = Cast<ULocomotionComponent>(GetOuter());
	int startNode = GetClosestNodeToLocation(pLocomotion->GetCharacter()->GetActorLocation());

	// 2. Get closest node to goal location
	int targetNode = GetClosestNodeToLocation(InTargetLocation);

	// 3. AStar search
	//AStar search(grid, startNode, targetNode);

	// 4. Grab the path
	//OutPath = search.GetPath();

	return false;
}

int UPathPlanner::GetClosestNodeToLocation(FVector InLocation)
{
	return 0;
}
