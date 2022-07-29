// Fill out your copyright notice in the Description page of Project Settings.


#include "Locomotion/LocomotionComponent.h"

#include "GameFramework/Character.h"
#include "Locomotion/Path.h"
#include "Engine/TargetPoint.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
ULocomotionComponent::ULocomotionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
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

// Called when the game starts
void ULocomotionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void ULocomotionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
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
