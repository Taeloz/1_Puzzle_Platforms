// Fill out your copyright notice in the Description page of Project Settings.

#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}

	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		FVector Location = GetActorLocation();

		float journeyLength = (GlobalTargetLocation - GlobalStartLocation).Size();
		float journeyTravelled = (Location - GlobalStartLocation).Size();
		if (journeyTravelled >= journeyLength)
		{
			FVector NewStartLocation = GlobalTargetLocation;
			GlobalTargetLocation = GlobalStartLocation;
			GlobalStartLocation = NewStartLocation;
		}

		FVector MoveVector = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal() * speed * DeltaTime;
		Location += MoveVector;
		SetActorLocation(Location);
	}
}
