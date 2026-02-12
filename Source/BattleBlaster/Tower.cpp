// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"

void ATower::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle FireTimerHandler;
	GetWorldTimerManager().SetTimer(FireTimerHandler, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsInFireRange())
	{
		RotateTurret(Tank->GetActorLocation());
	}
}

void ATower::SetTank(ATank* NewTank)
{
	Tank = NewTank;
}

void ATower::CheckFireCondition()
{
	if (IsInFireRange())
	{
		Fire();
	}
}

bool ATower::IsInFireRange()
{
	if (Tank)
	{
		FVector TankLocation = Tank->GetActorLocation();
		FVector TowerLocation = this->GetActorLocation();
		float DistanceToTank = FVector::Dist(TowerLocation, TankLocation);
		if (DistanceToTank < FireRange)
		{
			return true;
		}
	}
	return false;
}
