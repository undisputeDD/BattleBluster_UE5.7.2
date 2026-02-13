// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tower.h"
#include "Tank.h"

void ABattleBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), Towers);
	TowerCount = Towers.Num();
	UE_LOG(LogTemp, Display, TEXT("Number of Towers: %d"), TowerCount);
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		Tank = Cast<ATank>(PlayerPawn);
		if (!Tank)
		{
			UE_LOG(LogTemp, Error, TEXT("GameMode: failed to find the Tank actor"));
		}
		else
		{
			for (int i = 0; i < TowerCount; ++i)
			{
				ATower* CurrentTower = Cast<ATower>(Towers[i]);
				if (CurrentTower)
				{
					CurrentTower->SetTank(Tank);
					UE_LOG(LogTemp, Display, TEXT("%s assigning tank"), *CurrentTower->GetActorNameOrLabel());
				}
			}
		}
	}
}

void ABattleBlasterGameMode::ActorDied(AActor* DamagedActor)
{
	if (DamagedActor == Tank)
	{
		Tank->HandleDestruction();
	}
	else
	{
		ATower* DeadTower = Cast<ATower>(DamagedActor);
		if (DeadTower)
		{
			DeadTower->HandleDestruction();
			--TowerCount;
			if (TowerCount <= 0)
			{
				UE_LOG(LogTemp, Display, TEXT("All Towers destroyed, victory!"));
			}
		}
	}
}
