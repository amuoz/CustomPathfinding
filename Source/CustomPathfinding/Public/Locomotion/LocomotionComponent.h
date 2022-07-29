// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "LocomotionComponent.generated.h"

class APath;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CUSTOMPATHFINDING_API ULocomotionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	// Sets default values for this component's properties
	ULocomotionComponent();

	// Prioritized steering combination
	FVector Calculate();

	void SetGoalLocation(FVector InGoalLocation);

	UFUNCTION(BlueprintCallable)
	void SetPath(APath* InPath);

protected:

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	const ACharacter* GetCharacter() const;

	FVector Seek(FVector TargetLocation);

	FVector FollowPath();

private:

	UPROPERTY(replicated)
	FVector GoalLocation = FVector::ZeroVector;

	UPROPERTY()
	APath* m_pPath = nullptr;

	UPROPERTY()
	double m_WaypointDistSq = 100.f;
};

