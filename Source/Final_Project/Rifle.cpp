// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "Rifle.h"
#include "Components/ArrowComponent.h"
#include "Projectile.h"

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

// Called when the game starts or when spawned
void ARifle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARifle::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

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
		UWorld* const World = GetWorld();
		if (World)
		{
			// Required by SpawnActor function
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			// Get the Location of the Arrow component which defines the gun muzzle location
			FVector SpawnLocation = ArrowComp->GetComponentTransform().GetLocation();
			// Get the Rotation of the Arrow component which defines the gun muzzle location
			FRotator SpawnRotation = (FRotator)ArrowComp->GetComponentTransform().GetRotation();

			// Spawn a Projectile object
			AProjectile* const SpawnedProjectile = World->SpawnActor<AProjectile>(
				SpawnLocation,	// Location to the Arrow
				SpawnRotation,	// Rotation of the Arrow
				SpawnParams		// Spawn Paramaters
				);
			if (SpawnedProjectile != NULL)
			{
				// Projectile is spawned, decrement AmmoCount
				AmmoCount--;
			}
		}
	}
}

int ARifle::CalculateAmmo()
{
	// No Ammo
	if (AmmoCount <= 0)
		return AmmoCount = 0;

	return AmmoCount;
}

void ARifle::SetAmmoCount(int Delta){ AmmoCount = Delta; }

int ARifle::GetClipSize(){ return ClipSize; }