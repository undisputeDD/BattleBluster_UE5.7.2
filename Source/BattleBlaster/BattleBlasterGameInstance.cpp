// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UBattleBlasterGameInstance::ChangeLevel(int32 Index)
{
	if (Index > 0 && Index <= MaxLevelIndex)
	{
		CurrentLevelIndex = Index;
		UGameplayStatics::OpenLevel(GetWorld(), *FString::Printf(TEXT("Level_%d"), CurrentLevelIndex));
	}
}

void UBattleBlasterGameInstance::LoadNextLevel()
{
	if (CurrentLevelIndex == MaxLevelIndex)
	{
		RestartGame();
	}
	else
	{
		ChangeLevel(CurrentLevelIndex + 1);
	}
}

void UBattleBlasterGameInstance::RestartCurrentLevel()
{
	ChangeLevel(CurrentLevelIndex);
}

void UBattleBlasterGameInstance::RestartGame()
{
	ChangeLevel(1);
}
