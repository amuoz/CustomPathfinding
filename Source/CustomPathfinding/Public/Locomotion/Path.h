// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Path.generated.h"

class ATargetPoint;

UCLASS()
class CUSTOMPATHFINDING_API APath : public AActor
{
	GENERATED_BODY()
	
public:	

	APath();

	const ATargetPoint* GetCurrentWaypoint() const;

	void Init();

	void SetNextWaypoint();

	bool IsFinished() const;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<ATargetPoint*> Waypoints;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:	

	UPROPERTY()
	ATargetPoint* m_pCurrentWaypoint = nullptr;

};
