// Fill out your copyright notice in the Description page of Project Settings.


#include "Locomotion/LocomotionComponent.h"

#include "GameFramework/Character.h"
#include "Locomotion/Path.h"
#include "Engine/TargetPoint.h"
#include "Net/UnrealNetwork.h"
#include "Locomotion/PathPlanner.h"
#include "Kismet/GameplayStatics.h"
#include "Locomotion/Navigation/Grid.h"

DEFINE_LOG_CATEGORY_STATIC(LogLocomotionComponent, Log, All);

// Sets default values for this component's properties
ULocomotionComponent::ULocomotionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);	
}

FVector ULocomotionComponent::Calculate()
{
	if (m_pPath != nullptr)
	{
		return FollowPath();
	}

	if (GoalLocation != FVector::ZeroVector)
	{
		return Seek(GoalLocation);
	}

	return FVector::ZeroVector;
}

void ULocomotionComponent::SetGoalLocation(FVector InGoalLocation)
{
	GoalLocation = InGoalLocation;
}

void ULocomotionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Init path planner only if grid navigation instance found
	TArray<AActor*> foundGrids;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGrid::StaticClass(), foundGrids);
	if (foundGrids.Num() > 0)
	{
		if (const AGrid* worldGrid = Cast<AGrid>(foundGrids[0]))
		{
			m_pPathPlanner = NewObject<UPathPlanner>(GetOuter(), UPathPlanner::StaticClass());
			m_pPathPlanner->Init(worldGrid);
		}
	}
}

void ULocomotionComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ULocomotionComponent, GoalLocation);
}

const ACharacter* ULocomotionComponent::GetCharacter() const
{
	if (GetOwner<AController>() != nullptr)
	{
		if (const ACharacter* pCharacter = Cast<ACharacter>(GetOwner<AController>()->GetPawn()))
		{
			return pCharacter;
		}
	}
	return nullptr;
}

FVector ULocomotionComponent::Seek(FVector TargetLocation)
{
	const ACharacter* pCharacter = GetCharacter();
	if (pCharacter == nullptr)
	{
		return FVector::ZeroVector;
	}

	return (TargetLocation - pCharacter->GetActorLocation()).GetSafeNormal();
}

void ULocomotionComponent::SetPath(APath* InPath)
{
	m_pPath = InPath;
	m_pPath->Init();
}

FVector ULocomotionComponent::FollowPath()
{
	const ACharacter* pCharacter = GetCharacter();
	if (pCharacter == nullptr || m_pPath == nullptr)
	{
		return FVector::ZeroVector;
	}

	if(m_pPath->IsFinished())
	{
		m_pPath = nullptr;
		return FVector::ZeroVector;
	}

	double distanceToWaypoint = FVector::Distance(m_pPath->GetCurrentWaypoint()->GetActorLocation(), pCharacter->GetActorLocation());
	if (distanceToWaypoint < m_WaypointDistSq)
	{
		m_pPath->SetNextWaypoint();
	}

	if (!m_pPath->IsFinished())
	{
		return Seek(m_pPath->GetCurrentWaypoint()->GetActorLocation());
	}
	
	return FVector::ZeroVector;
}
