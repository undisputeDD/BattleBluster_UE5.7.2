// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ATank::ATank()
{
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm Component"));
	SpringArmComp->SetupAttachment(RootComponent);
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController)
	{
		if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
	IsAlive = true;
	SetPlayerEnabled(false);
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerController)
	{
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		RotateTurret(HitResult.ImpactPoint);
	}
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATank::MoveInput);
		EIC->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ATank::RotateInput);
		EIC->BindAction(FireAction, ETriggerEvent::Started, this, &ATank::Fire);
	}
}

void ATank::MoveInput(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();
	FVector DeltaLocation;
	DeltaLocation.X = Speed * InputValue * GetWorld()->GetDeltaSeconds();
	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::RotateInput(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();
	FRotator DeltaRotation;
	DeltaRotation.Yaw = RotateRate * InputValue * GetWorld()->GetDeltaSeconds();
	AddActorLocalRotation(DeltaRotation, true);
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();
	UE_LOG(LogTemp, Display, TEXT("Tank died."));
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	SetPlayerEnabled(false);
	IsAlive = false;
}

void ATank::SetPlayerEnabled(bool Enabled)
{
	if (PlayerController)
	{
		if (Enabled)
		{
			EnableInput(PlayerController);
		}
		else
		{
			DisableInput(PlayerController);
		}
		PlayerController->SetShowMouseCursor(Enabled);
	}
}

bool ATank::GetIsAlive()
{
	return IsAlive;
}
