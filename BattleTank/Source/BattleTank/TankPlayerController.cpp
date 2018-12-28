// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Tank.h"
#include "Engine/World.h"

ATank* ATankPlayerController::GetControlledTank() const 
{
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardCrosshair()
{
	if (!GetControlledTank()) { return; }

	FVector TargetLocation;

	if (GetSightRayHitLocation(TargetLocation))
	{
		GetControlledTank()->AimAt(TargetLocation);
	}
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
	HitLocation = FVector(1.0);
	
	FVector AimDirection;
	if (GetAimDirection(AimDirection))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Direction: %s"), *(AimDirection.ToString()));
		return GetAimDirectionHitLocation(AimDirection, HitLocation);
	}

	return false;
}

bool ATankPlayerController::GetAimDirection(FVector& AimDirection) const
{
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	auto ScreenLocation = FVector2D(ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation);

	FVector CameraLocation;
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraLocation, AimDirection);

}

bool ATankPlayerController::GetAimDirectionHitLocation(FVector AimDirection, FVector& HitLocation) const
{
	auto CameraLocation = PlayerCameraManager->GetCameraLocation();

	FHitResult HitResult;
	if (GetWorld()->LineTraceSingleByChannel(
		HitResult, 
		CameraLocation,
		CameraLocation + AimDirection * LineTraceRange,
		ECollisionChannel::ECC_Visibility))
	{
		HitLocation = HitResult.Location;
		//UE_LOG(LogTemp, Warning, TEXT("HIT: %s"), *HitLocation.ToString());

		return true;
	}

	return false;
}

void ATankPlayerController::BeginPlay() 
{
	Super::BeginPlay();

	auto ControlledTank = GetControlledTank();

	if (!ControlledTank) {
		UE_LOG(LogTemp, Warning, TEXT("No Controlled Tank!"));

	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Controlling %s"), *ControlledTank->GetName());
	}
}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AimTowardCrosshair();
}