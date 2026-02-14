// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BattleBlasterGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API UBattleBlasterGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	void LoadNextLevel();
	void RestartCurrentLevel();
	void RestartGame();

private:
	void ChangeLevel(int32 Index);

private:
	UPROPERTY(EditAnywhere)
	int32 MaxLevelIndex = 3;
	UPROPERTY(VisibleAnywhere)
	int32 CurrentLevelIndex = 1;
};
