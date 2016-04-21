// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "Components/ArrowComponent.h"
#include "Projectile.h"
#include "IDamageable.h"
#include "BaseCharacter.h"
#include "FuzzyModule.h"
#include "BaseWeapon.h"

// Sets default values
ABaseWeapon::ABaseWeapon(){}

double ABaseWeapon::CalculateDesirability(double Dist, double Ammo){ return 0.0; }

void ABaseWeapon::SetupWeaponMesh(USkeletalMesh* Mesh, UMaterial* Mat, FVector relLocation, FRotator relRotation)
{
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun_Mesh"));
	GunMesh->AttachTo(RootComponent);
	GunMesh->SetSkeletalMesh(Mesh);
	GunMesh->SetMaterial(0, Mat);
	GunMesh->SetRelativeLocation(relLocation);
	GunMesh->SetRelativeRotation(relRotation);
	GunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// Set non collidable
}

void ABaseWeapon::SetupArrowComp(FVector relLocation, FRotator relRotation)
{
	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Weapon_Muzzle"));
	ArrowComp->AttachTo(GunMesh);
	ArrowComp->SetRelativeLocation(relLocation);
	ArrowComp->SetRelativeRotation(relRotation);
	ArrowComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseWeapon::TraceLine(FHitResult* Hit, bool DrawTrace)
{
	// Get the location of the arrow component
	FVector Loc = ArrowComp->GetComponentLocation();
	// The direction is equal to the rotation
	FVector Direction = ArrowComp->GetForwardVector();

	// Starting point of line trace
	FVector Start = FVector(0.0f, 0.0f, 0.0f);
	Start = Loc + Direction;

	// Distance of line trace
	const float Scalar = Range;
	// End point of the line trace
	FVector End = Start + Direction * Scalar;

	// Name of the line trace
	static FName TraceIdentifier = FName(TEXT("Weapon_Trace"));
	// Default trace params
	FCollisionQueryParams TraceParams(TraceIdentifier, false, this);
	TraceParams.bTraceAsyncScene = true;

	// Create the line trace
	UWorld* World = GetWorld();
	if (World)
	{
		World->LineTraceSingleByChannel(*Hit, Start, End, ECollisionChannel::ECC_Visibility, TraceParams, FCollisionResponseParams::DefaultResponseParam);
		// Should the trace be visible
		if (DrawTrace)
			// Visualise the line trace
			DrawDebugLine(World, Start, End, FColor::Red, false, -1, 0, 2.0f);
	}
}

bool ABaseWeapon::PullTrigger()
{
	Fire();
	// Start Timer and run Fire() function
	UWorld* World = GetWorld();
	if (World)
	{
		// If timer does not exist
		if ((!TimerHandle.IsValid()))
		{
			World->GetTimerManager().SetTimer(TimerHandle, this, &ABaseWeapon::Fire, 1.0f / RPS, true);
		}
	}
	// Trigger pulled
	return true;
}

bool ABaseWeapon::ReleaseTrigger()
{
	// Clear Timer
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(TimerHandle);
		// Invalidate the tiemrhandle so the timer can be set again in the PullTrigger function
		TimerHandle.Invalidate();
	}
	// Trigger not pulled
	return false;
}

void ABaseWeapon::Fire()
{
	// Can only fire if there is available ammo
	if (CalculateAmmo() > 0)
	{
		FHitResult Hit(ForceInit);
		// Fire a single line trace
		TraceLine(&Hit, true);

		// If the line trace hits an actor, store it.
		AActor* Other = Hit.GetActor();
		if (Other != NULL)
		{
			// Check the actor implements the IDamageable interface
			if (Other->GetClass()->ImplementsInterface(UIDamageable::StaticClass()))
			{
				/*
				* OtherActor is of BaseCharacter class type so create cast
				* So that we can access the class' functions
				*/
				ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(Other);
				if (BaseCharacter->ActorHasTag("Player") && BaseCharacter != NULL)
				{
					// Do less damage to the player
					BaseCharacter->DealDamage_Implementation(PlayerDamage);
				}
				// Check that the owner is not also an enemy
				else if (!(GetOwner()->ActorHasTag("Enemy")) && (BaseCharacter != NULL))
				{
					// Deal damage to the base character i.e. Agent
					BaseCharacter->DealDamage_Implementation(EnemyDamage);
				}
			}
		}
		AmmoCount--;
	}
}

int32 ABaseWeapon::CalculateAmmo()
{
	// No Ammo
	if (AmmoCount <= 0)
		return AmmoCount = 0;

	return AmmoCount;
}

void ABaseWeapon::SetRPS(float Val){ RPS = Val; }
float ABaseWeapon::GetRPS(){ return RPS; }
void ABaseWeapon::SetRange(float Val){ Range = Val; }
float ABaseWeapon::GetRange(){ return Range; }
void ABaseWeapon::SetClipSize(int32 Val){ ClipSize = Val; }
int32 ABaseWeapon::GetClipSize(){ return ClipSize; }
void ABaseWeapon::SetAmmoCount(int32 Val){ AmmoCount = Val; }
void ABaseWeapon::SetPlayerDamage(float Val){ PlayerDamage = Val; }
float ABaseWeapon::GetPlayerDamage(){ return PlayerDamage; }
void ABaseWeapon::SetEnemyDamage(float Val){ EnemyDamage = Val; }
float ABaseWeapon::GetEnemyDamage(){ return EnemyDamage; }