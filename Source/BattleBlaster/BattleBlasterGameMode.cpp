// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tower.h"
#include "Tank.h"
#include "BattleBlasterGameInstance.h"
#include "ScreenMessage.h"

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

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		ScreenMessageWidget = CreateWidget<UScreenMessage>(PlayerController, ScreenMessageClass);
		if (ScreenMessageWidget)
		{
			ScreenMessageWidget->SetMessageText("Get Ready!");
			ScreenMessageWidget->AddToPlayerScreen();
		}
	}

	CountdownSeconds = CountdownDelta;
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, this, &ABattleBlasterGameMode::OnCountDownTimerTimeout, 1.0f, true);
}

void ABattleBlasterGameMode::ActorDied(AActor* DamagedActor)
{
	bool IsGameOver = false;

	if (DamagedActor == Tank)
	{
		Tank->HandleDestruction();
		IsGameOver = true;
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
				IsGameOver = true;
				IsVictory = true;
			}
		}
	}

	if (IsGameOver)
	{
		FString GameOverString = IsVictory ? "Victory" : "Defeat";
		UE_LOG(LogTemp, Display, TEXT("Game is over: %s!"), *GameOverString);
		ScreenMessageWidget->SetMessageText(FString::Printf(TEXT("Game is over : %s!"), *GameOverString));
		ScreenMessageWidget->SetVisibility(ESlateVisibility::Visible);
		FTimerHandle GameOverTimerHandler;
		GetWorldTimerManager().SetTimer(GameOverTimerHandler, this, &ABattleBlasterGameMode::OnGameOverTimerTimeout, GameOverTimeout, false);
	}
}

void ABattleBlasterGameMode::OnGameOverTimerTimeout()
{
	UE_LOG(LogTemp, Display, TEXT("Game over timeout!"));
	UBattleBlasterGameInstance* BBGI = Cast<UBattleBlasterGameInstance>(GetGameInstance());
	if (IsVictory)
	{
		BBGI->LoadNextLevel();
	}
	else
	{
		BBGI->RestartCurrentLevel();
	}
}

void ABattleBlasterGameMode::OnCountDownTimerTimeout()
{
	--CountdownSeconds;
	if (CountdownSeconds > 0)
	{
		ScreenMessageWidget->SetMessageText(FString::Printf(TEXT("%d"), CountdownSeconds));
	}
	else if (CountdownSeconds == 0)
	{
		ScreenMessageWidget->SetMessageText("Go!");
		Tank->SetPlayerEnabled(true);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(CountdownTimerHandle);
		UE_LOG(LogTemp, Display, TEXT("Clear timer"));
		ScreenMessageWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
