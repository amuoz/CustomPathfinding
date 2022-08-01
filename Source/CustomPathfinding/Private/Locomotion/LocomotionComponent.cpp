// Fill out your copyright notice in the Description page of Project Settings.


#include "Locomotion/LocomotionComponent.h"

#include "GameFramework/Character.h"
#include "Locomotion/Navigation/Path.h"
#include "Engine/TargetPoint.h"
#include "Net/UnrealNetwork.h"
#include "Locomotion/Navigation/PathPlanner.h"
#include "Kismet/GameplayStatics.h"
#include "Locomotion/Navigation/Grid.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogLocomotionComponent, Log, All);

ULocomotionComponent::ULocomotionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
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

void ULocomotionComponent::SetGoalLocation(FVector GoalLocation)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		UE_LOG(LogLocomotionComponent, Log, TEXT("Set Goal Location: %s"), *GoalLocation.ToString());

		// Ask Path Planner to find a valid Path
		TArray<FVector> outPath;
		if (m_pPathPlanner->CreatePathToLocation(GetCharacter()->GetActorLocation(), GoalLocation, outPath))
		{
			if (bDebug)
			{
				for (auto It = outPath.CreateConstIterator(); It; ++It)
				{
					DrawDebugSphere(
						GetOwner()->GetWorld(),
						*It,
						30.f,
						10,
						FColor::Yellow,
						false,
						2.f,
						0,
						2.f
					);
				}
			}

			// Sets new path for path following
			APath* pPath = NewObject<APath>(GetOuter(), APath::StaticClass());
			pPath->Waypoints = outPath;
			SetPath(pPath);
		}
	}
}

const FVector ULocomotionComponent::GetMoveDirection() const
{
	return m_MoveDirection;
}

bool ULocomotionComponent::CheckCharacterCanWalk(FVector StartLocation, FVector EndLocation)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	TArray<AActor*> IgnoredActors;
	FHitResult OutHit;
	StartLocation.Z = 0;
	EndLocation.Z = 0;
	EDrawDebugTrace::Type drawDebug = bDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

	bool bHit = UKismetSystemLibrary::CapsuleTraceSingleForObjects(
		GetOwner()->GetWorld(),
		StartLocation + FVector(0.f, 0.f, GetCharacter()->GetDefaultHalfHeight()),
		EndLocation + FVector(0.f, 0.f, GetCharacter()->GetDefaultHalfHeight()),
		GetCharacter()->GetSimpleCollisionRadius(),
		GetCharacter()->GetDefaultHalfHeight(),
		ObjectTypesArray,
		false,
		IgnoredActors,
		drawDebug,
		OutHit,
		true,
		FLinearColor::Red, 
		FLinearColor::Red, 
		2.f);

	return !bHit;
}

void ULocomotionComponent::SetSmoothing(bool NewSmoothing)
{
	bSmoothing = NewSmoothing;
	m_pPathPlanner->SetSmoothing(bSmoothing);
}

void ULocomotionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Init path planner only if grid navigation instance found
	TArray<AActor*> foundGrids;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGrid::StaticClass(), foundGrids);
	if (foundGrids.Num() == 1)
	{
		if (AGrid* worldGrid = Cast<AGrid>(foundGrids[0]))
		{
			m_pPathPlanner = NewObject<UPathPlanner>(this, UPathPlanner::StaticClass());
			m_pPathPlanner->Init(worldGrid, bSmoothing);
		}
	}
	else
	{
		UE_LOG(LogLocomotionComponent, Error, TEXT("No Grid found or multiple Grids found!"));
	}
}

void ULocomotionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Movement driven by the server
	if (GetOwnerRole() == ROLE_Authority)
	{
		m_MoveDirection = Calculate();
	}
}

void ULocomotionComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ULocomotionComponent, m_MoveDirection);
}

FVector ULocomotionComponent::Calculate()
{
	if (m_pPath != nullptr)
	{
		return FollowPath();
	}
	return FVector::ZeroVector;
}

void ULocomotionComponent::SetPath(APath* InPath)
{
	m_pPath = InPath;
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

	double distanceToWaypoint = FVector::Dist2D(m_pPath->GetCurrentWaypoint(), pCharacter->GetActorLocation());
	if (distanceToWaypoint < WaypointDistanceSq)
	{
		m_pPath->SetNextWaypoint();
	}

	if (!m_pPath->IsFinished())
	{
		return Seek(m_pPath->GetCurrentWaypoint());
	}
	
	return FVector::ZeroVector;
}
