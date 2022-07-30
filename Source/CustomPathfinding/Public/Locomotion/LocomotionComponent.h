// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "LocomotionComponent.generated.h"


class APath;
class UPathPlanner;

UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class CUSTOMPATHFINDING_API ULocomotionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// Sets default values for this component's properties
	ULocomotionComponent();

	// Get controlled character
	const ACharacter* GetCharacter() const;

	// Prioritized steering combination
	FVector Calculate();

	void SetGoalLocation(FVector InGoalLocation);

	UFUNCTION(BlueprintCallable)
	void SetPath(APath* InPath);

	const FVector GetMoveDirection() const;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	FVector Seek(FVector TargetLocation);

	FVector FollowPath();


private:

	UPROPERTY()
	UPathPlanner* m_pPathPlanner;

	UPROPERTY(replicated)
	FVector m_GoalLocation = FVector::ZeroVector;

	UPROPERTY(replicated)
	FVector m_MoveDirection = FVector::ZeroVector;

	UPROPERTY()
	APath* m_pPath = nullptr;

	UPROPERTY(EditAnywhere)
	double m_WaypointDistSq = 50.f;
};

