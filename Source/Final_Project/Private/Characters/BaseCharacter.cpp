// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "BaseCharacter.h"
#include "Rifle.h"
#include "Shotgun.h"
#include "BaseWeapon.h"
#include "Engine.h"
#include "Delegate.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;	// Set the default movement speed

	GetMesh()->DestroyComponent(); // Destroy old inherited mesh
	// Replace with new mesh
	ManMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mannequin_Mesh"));
	ManMesh->AttachTo(RootComponent);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin"));
	if (MeshAsset.Succeeded())
	{
		// Set Mesh via code
		ManMesh->SetSkeletalMesh(MeshAsset.Object);
		ManMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -97.0f));
		ManMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		ManMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);	// Set collidable
		ManMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block); // Set the visibility channel to block
		ManMesh->bGenerateOverlapEvents = true; // Allow other actors to overlap
		ManMesh->SetNotifyRigidBodyCollision(true); // Set 'Simulation Generates Hit Events' true so the trace line can collide

		// Set the animation blueprint to be used
		static ConstructorHelpers::FClassFinder<UAnimInstance> AnimationAsset(TEXT("/Game/AnimStarterPack/UE4ASP_HeroTPP_AnimBlueprint"));
		if (AnimationAsset.Succeeded())
		{
			// Assign the animation blueprint to the mesh
			ManMesh->SetAnimInstanceClass(AnimationAsset.Class);
		}
		// Set the material
		static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/AnimStarterPack/UE4_Mannequin/Materials/M_UE4Man_Body"));
		static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialAsset01(TEXT("/Game/AnimStarterPack/UE4_Mannequin/Materials/M_UE4Man_ChestLogo"));
		if (MaterialAsset.Succeeded())
		{
			ManMesh->SetMaterial(0, MaterialAsset.Object);
			ManMesh->SetMaterial(1, MaterialAsset01.Object);
		}
		// Set the physics asset
		static ConstructorHelpers::FObjectFinder<UPhysicsAsset> PhysicsAsset(TEXT("/Game/AnimStarterPack/UE4_Mannequin/Mesh/Physics_Asset"));
		if (PhysicsAsset.Succeeded())
		{
			ManMesh->PhysicsAssetOverride = PhysicsAsset.Object;
		}
	}
}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
 
	// Spawn Rifle in when game starts
	UWorld* const World = GetWorld();
	if (World)
	{
		// Required by SpawnActor function
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;

		// Spawn a Rifle object
		SpawnedRifle = World->SpawnActor<ARifle>(SpawnParams);
		if (SpawnedRifle != NULL)
		{
			SpawnedRifle->AttachRootComponentTo(ManMesh, TEXT("GunSocket"), EAttachLocation::SnapToTargetIncludingScale, true);
			CurrentWeapon = SpawnedRifle;
		}
	}
}

// Called every frame
void ABaseCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	Time = DeltaTime;
	// Get the new rotation
	DeltaRotator = GetControlRotation() - GetActorRotation();

	// As a fail safe if the Fire() function is continuosly called,
	// it will still fire while reloading, which we do not want to happen.
	if (!CanFire)
	{
		StopFiring();
	}
}

void ABaseCharacter::CustomRInterpTo(FRotator current, float Interp_Speed)
{
	FRotator InterpRot = FMath::RInterpTo(current, DeltaRotator, Time, Interp_Speed);

	// Break the rotator into pitch and yaw
	NewYaw = FMath::ClampAngle(InterpRot.Yaw, -90.0f, 90.0f);
	NewPitch = FMath::ClampAngle(InterpRot.Pitch, -90.0f, 90.0f);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

USkeletalMeshComponent* ABaseCharacter::GetNewMesh() const { return ManMesh; }

void ABaseCharacter::Sprint()
{
	// We cannot sprint if we are:
	if (Controller != NULL 
		&& !isCrouching		// Crouching
		&& !isAiming		// Aiming
		&& !isProne)		// Or prone
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void ABaseCharacter::StopSprinting()
{
	if (Controller != NULL)
	{
		GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
	}
}

void ABaseCharacter::Crouch(){ isCrouching = true; }

void ABaseCharacter::UnCrouch(){ isCrouching = false; }

void ABaseCharacter::GoProne(){ isProne = true; }

void ABaseCharacter::Stand(){ isProne = false; }

void ABaseCharacter::Jump(){ isJumping = true; }

void ABaseCharacter::StopJumping(){ isJumping = false; }

void ABaseCharacter::AimDownSight()
{
	// We cannot aim if we are sprinting
	if (Controller != NULL && !(GetCharacterMovement()->MaxWalkSpeed >= SprintSpeed) )
	{
		isAiming = true;
		// Slow to a walk
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void ABaseCharacter::StopAiming()
{ 
	if (Controller != NULL)
	{
		isAiming = false;
		// Go back to normal speed
		GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
	}
}

void ABaseCharacter::Fire()
{ 
	if (CanFire)
	{
		if (GetCurrentWeapon() != NULL)
			isFiring = GetCurrentWeapon()->PullTrigger();
	}
}

void ABaseCharacter::StopFiring()
{
	if (GetCurrentWeapon() != NULL)
		isFiring = GetCurrentWeapon()->ReleaseTrigger();
}

ABaseWeapon* ABaseCharacter::GetCurrentWeapon(){ return CurrentWeapon; }

void ABaseCharacter::SetCurrentWeapon(ABaseWeapon* Weapon){ CurrentWeapon = Weapon; }

void ABaseCharacter::BeginReload()
{ 
	CanFire = false;
	isReloading = true; 
	// Reload
	if (GetCurrentWeapon() != NULL)
		// Reset AmmoCount
		GetCurrentWeapon()->SetAmmoCount(GetCurrentWeapon()->GetClipSize());
}

void ABaseCharacter::EndReload(){ isReloading = false; }

void ABaseCharacter::CalcHealth(float DamageDealt)
{
	HealthWeight -= DamageDealt;	// Take damage off health
	
	// If the character has no health left
	if (HealthWeight <= 0)
	{
		// Check the controller has not already been detached for whatever reason
		if (Controller != NULL)
			// Stop character from being able to move so the death animation can be played
			DetachFromControllerPendingDestroy();
		
		// Disable collision on the character
		ManMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// We can now play the death animation
		isDead = true;

		if (!TimerHandle.IsValid())
			// Destroy the character after a set interval
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ABaseCharacter::DelayedDestroy, 3.0f, true);
	}
	else // If the character still has some health remaining
	{
		isDead = false;
	}
}

void ABaseCharacter::DelayedDestroy()
{
	GetWorldTimerManager().ClearTimer(TimerHandle);
	TimerHandle.Invalidate();

	// Ensure the weapon associated with this characer is also destroyed
	if (GetCurrentWeapon() != NULL)
		GetCurrentWeapon()->Destroy();

	// Destroy the character
	Destroy();
}

void ABaseCharacter::DealDamage_Implementation(float Damage)
{
	/* Hit animation interrupts reload animation end AnimNotify event.
	Causing CanFire to not be reset to true. So check if CanFire needs
	to be reset, if it does, reset it. 
	This is a bit of a hack, but it works so...*/
	if (!CanFire) CanFire = true;
	// If we have not already been hit.
	if (!bReceivedHit)
		// The animation can play
		bReceivedHit = true;
	/* Otherwise bReceivedHit is true and the hit animation will
	play for as many times as we have been hit. But if we're already playing 
	the animation we do not want to play it again because it will interrupt other events. */
	else
	{
		// Don't allow the animation to play again
		bReceivedHit = false;
	}
	CalcHealth(DealDamage(Damage));
}
