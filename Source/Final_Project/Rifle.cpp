// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "Rifle.h"
#include "Components/ArrowComponent.h"
#include "Projectile.h"
#include "IDamageable.h"
#include "BaseCharacter.h"

// Sets default values
ARifle::ARifle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun_Mesh"));
	GunMesh->AttachTo(RootComponent);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GunAsset(TEXT("/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun"));
	if (GunAsset.Succeeded())
	{
		GunMesh->SetSkeletalMesh(GunAsset.Object);
		GunMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		GunMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		GunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// Set non collidable

		static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/FPWeapon/Materials/M_FPGun"));
		if (MaterialAsset.Succeeded())
		{
			GunMesh->SetMaterial(0, MaterialAsset.Object);
		}

		ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Projectile_Spawn_Point"));
		ArrowComp->AttachTo(GunMesh);
		ArrowComp->SetRelativeLocation(FVector(0.0f, 50.0f, 11.0f));
		ArrowComp->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		ArrowComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ARifle::TraceLine(FHitResult* Hit)
{
	// Get the location of the arrow component
	FVector Loc = ArrowComp->GetComponentLocation();
	// Get the rotation of the arrow component
	FRotator Rot = ArrowComp->GetComponentRotation();
	// The direction is equal to the rotation
	FVector Direction = ArrowComp->GetForwardVector();

	// Starting point of line trace
	FVector Start = FVector(0.0f, 0.0f, 0.0f);
	Start = Loc + Direction;

	// Distance of line trace
	const float Scalar = 4000.0f;
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
		// Visualise the line trace
		DrawDebugLine(World, Start, End, FColor::Red, false, -1, 0, 2.0f);
	}
}

bool ARifle::PullTrigger()
{
	Fire();
	// Start Timer and run Fire() function
	UWorld* World = GetWorld();
	if (World)
	{
		// If timer does not exist
		if ((!TimerHandle.IsValid()))
		{
			World->GetTimerManager().SetTimer(TimerHandle, this, &ARifle::Fire, 1.0f / RPS, true);
		}
	}
	// Trigger pulled
	return true;
}

bool ARifle::ReleaseTrigger()
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

void ARifle::Fire()
{
	// Can only fire if there is available ammo
	if (CalculateAmmo() > 0)
	{
		FHitResult Hit(ForceInit);
		// Fire a single line trace
		TraceLine(&Hit);

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
					BaseCharacter->DealDamage_Implementation(0.5f);
				}
				else if (BaseCharacter != NULL)
				{
					// Deal damage to the base character i.e. Agent
					BaseCharacter->DealDamage_Implementation(20.0f);
				}
			}
		}
		AmmoCount--;
	}
}

int32 ARifle::CalculateAmmo()
{
	// No Ammo
	if (AmmoCount <= 0)
		return AmmoCount = 0;

	return AmmoCount;
}

void ARifle::SetAmmoCount(int Delta){ AmmoCount = Delta; }

int32 ARifle::GetClipSize(){ return ClipSize; }