// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimingComponent.h"
#include "Engine.h"
#include "TankBarrel.h"
#include "TankTurret.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::AimAt(FVector TargetLocation, float FiringSpeed)
{
	if (!Barrel || !Turret)
	{
		return;
	}

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("BarrelEnd"));

	bool HasAimSolution = UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaunchVelocity,
		StartLocation,
		TargetLocation,
		FiringSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);

	if (HasAimSolution) {
		auto AimDirection = OutLaunchVelocity.GetSafeNormal();
		UE_LOG(LogTemp, Warning, TEXT("%s aiming at %s"), *GetOwner()->GetName(), *AimDirection.ToString());

		MoveBarrelAndTurretTowards(AimDirection);
	}

	UE_LOG(LogTemp, Warning, TEXT("%f: %s"), GetWorld()->GetTimeSeconds(), (HasAimSolution ? TEXT("aim found") : TEXT("aim not found")));
}

void UTankAimingComponent::MoveBarrelAndTurretTowards(FVector AimDirection)
{
	auto BarrelRotation = Barrel->GetForwardVector().Rotation();
	auto TurretRotation = Turret->GetForwardVector().Rotation();
	auto AimRotation = AimDirection.Rotation();

	auto BarrelRotator = AimRotation - BarrelRotation;
	auto TurretRotator = AimRotation - TurretRotation;

	Barrel->Elevate(BarrelRotator.Pitch);
	Turret->Rotate(TurretRotator.Yaw);
}

void UTankAimingComponent::SetBarrelReference(UTankBarrel* BarrelComponent)
{
	Barrel = BarrelComponent;
}

void UTankAimingComponent::SetTurretReference(UTankTurret* TurretComponent)
{
	Turret = TurretComponent;
}