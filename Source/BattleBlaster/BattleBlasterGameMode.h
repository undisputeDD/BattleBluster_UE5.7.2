// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleBlasterGameMode.generated.h"

class ATank;
class UScreenMessage;

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API ABattleBlasterGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void ActorDied(AActor* DamagedActor);
	void OnGameOverTimerTimeout();
	void OnCountDownTimerTimeout();

private:
	ATank* Tank;
	int32 TowerCount;
	UPROPERTY(EditAnywhere)
	float GameOverTimeout = 3.0f;
	bool IsVictory = false;
	UPROPERTY(EditAnywhere)
	FTimerHandle CountdownTimerHandle;
	UPROPERTY(EditAnywhere)
	int32 CountdownDelta = 3;
	int32 CountdownSeconds;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UScreenMessage> ScreenMessageClass;

	UScreenMessage* ScreenMessageWidget;
};
