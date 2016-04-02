// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "BaseCharacter.h"
#include "Rifle.h"
#include "Animation/AnimInstance.h"
#include "Engine.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	GetCharacterMovement()->MaxWalkSpeed = defaultSpeed;	// Set the default movement speed

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(TEXT("/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin"));
	if (MeshAsset.Succeeded())
	{
		// Set Mesh via code
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -97.0f));
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// Set non collidable

		// Set the animation blueprint to be used
		static ConstructorHelpers::FClassFinder<UAnimInstance> AnimationAsset(TEXT("/Game/AnimStarterPack/UE4ASP_HeroTPP_AnimBlueprint"));
		if (AnimationAsset.Succeeded())
		{
			// Assign the animation blueprint to the mesh
			GetMesh()->SetAnimInstanceClass(AnimationAsset.Class);
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
			SpawnedRifle->AttachRootComponentTo(GetMesh(), TEXT("GunSocket"), EAttachLocation::SnapToTargetIncludingScale, true);
	}
}

// Called every frame
void ABaseCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	Time = DeltaTime;
	// Get the new rotation
	DeltaRotator = GetControlRotation() - GetActorRotation();
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

void ABaseCharacter::Sprint()
{
	if (Controller != NULL && !isCrouching)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void ABaseCharacter::StopSprinting()
{
	if (Controller != NULL)
	{
		GetCharacterMovement()->MaxWalkSpeed = defaultSpeed;
	}
}

void ABaseCharacter::Crouch(){ isCrouching = true; }

void ABaseCharacter::UnCrouch(){ isCrouching = false; }

void ABaseCharacter::GoProne(){ isProne = true; }

void ABaseCharacter::Stand(){ isProne = false; }

void ABaseCharacter::Jump(){ isJumping = true; }

void ABaseCharacter::StopJumping(){ isJumping = false; }

void ABaseCharacter::AimDownSight(){ isAiming = true; }

void ABaseCharacter::StopAiming(){ isAiming = false; }

void ABaseCharacter::Fire()
{ 
	if (SpawnedRifle != NULL)
		isFiring = SpawnedRifle->PullTrigger();
}

void ABaseCharacter::StopFiring()
{
	if (SpawnedRifle != NULL)
		isFiring = SpawnedRifle->ReleaseTrigger();
}

void ABaseCharacter::BeginReload()
{ 
	isReloading = true; 
	// Reload
	if (SpawnedRifle != NULL)
		// Reset AmmoCount
		SpawnedRifle->SetAmmoCount(SpawnedRifle->GetClipSize());
}

void ABaseCharacter::EndReload(){ isReloading = false; }
