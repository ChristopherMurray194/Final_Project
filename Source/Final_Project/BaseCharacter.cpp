// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "BaseCharacter.h"
#include "Rifle.h"
#include "Animation/AnimInstance.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

		/*	Check this instance is not the temporary/preview instance,
			Spawn only when placed in the world */
		if (!UObject::IsTemplate(RF_Transient))
		{
			UWorld* const World = GetWorld();
			if (World)
			{
				// Required by SpawnActor function
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Instigator;

				// Spawn a Rifle object
				ARifle* const SpawnedRifle = World->SpawnActor<ARifle>(SpawnParams);

				if (SpawnedRifle != NULL)
					SpawnedRifle->AttachRootComponentTo(GetMesh(), TEXT("GunSocket"), EAttachLocation::SnapToTargetIncludingScale, true);
			}
		}
	}
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABaseCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
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


