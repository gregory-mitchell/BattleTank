// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"

#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Tank.h"

ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

ATank* ATankAIController::GetPlayerTank() const
{
	return Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
}

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	auto PlayerTank = GetPlayerTank();

	if (!PlayerTank) {
		UE_LOG(LogTemp, Warning, TEXT("No Player Tank!"));

	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Player %s"), *PlayerTank->GetName());
	}
}

void ATankAIController::Tick(float DeltaTime)
{
	auto PlayerTank = GetPlayerTank();
	if (PlayerTank)
	{
		FVector PlayerLocation = PlayerTank->GetActorLocation();
		GetControlledTank()->AimAt(PlayerLocation);
	}
}