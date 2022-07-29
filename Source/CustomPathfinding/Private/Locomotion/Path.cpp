// Fill out your copyright notice in the Description page of Project Settings.


#include "Locomotion/Path.h"

// Sets default values
APath::APath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void APath::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

const ATargetPoint* APath::GetCurrentWaypoint() const
{
	return m_pCurrentWaypoint;
}

void APath::Init()
{
	m_pCurrentWaypoint = Waypoints[0];
}

bool APath::IsFinished() const
{
	return m_pCurrentWaypoint == nullptr;
}

void APath::SetNextWaypoint()
{
	int32 currentIndex = Waypoints.Find(m_pCurrentWaypoint);
	if (++currentIndex < Waypoints.Num())
	{
		m_pCurrentWaypoint = Waypoints[currentIndex];
	}
	else
	{
		m_pCurrentWaypoint = nullptr;
	}
}
