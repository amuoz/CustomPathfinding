// Fill out your copyright notice in the Description page of Project Settings.

#include "Locomotion/Navigation/Path.h"

APath::APath()
{
	PrimaryActorTick.bCanEverTick = false;
}

const FVector APath::GetCurrentWaypoint() const
{
	return Waypoints[m_CurrentWaypoint];
}

bool APath::IsFinished() const
{
	return m_CurrentWaypoint == Waypoints.Num();
}

void APath::SetNextWaypoint()
{
	++m_CurrentWaypoint;
}
