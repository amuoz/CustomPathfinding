// Fill out your copyright notice in the Description page of Project Settings.


#include "Locomotion/LocomotionComponent.h"

// Sets default values for this component's properties
ULocomotionComponent::ULocomotionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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

FVector ULocomotionComponent::Seek(FVector TargetLocation)
{
	if (const APawn* pawn = GetOwner<APlayerController>()->GetPawn())
	{
		return (TargetLocation - pawn->GetActorLocation()).GetSafeNormal();
	}
	return FVector::ZeroVector;
}
