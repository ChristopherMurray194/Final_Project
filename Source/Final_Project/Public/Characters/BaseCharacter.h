// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IDamageable.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class FINAL_PROJECT_API ABaseCharacter : public ACharacter, public IIDamageable
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	ABaseCharacter();

	UPROPERTY(EditAnywhere)
		class USkeletalMeshComponent* ManMesh;

	virtual void PostInitializeComponents();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// New GetMesh function to use
	USkeletalMeshComponent* GetNewMesh() const;

	// Current tick time
	UPROPERTY(BlueprintReadOnly)
		// Current tick time
		float Time;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	//==================Crouching===================//

	// Crouch button is pressed
	UPROPERTY(BlueprintReadOnly)
		bool isCrouching = false;

	void Crouch();
	void UnCrouch();
	//==============================================//

	//===================Prone======================//

	// Prone key event fired
	UPROPERTY(BlueprintReadOnly)
		bool isProne = false;

	virtual void GoProne();
	void Stand();
	//==============================================//

	//===================Jumping====================//

	// Jump button is pressed
	UPROPERTY(BlueprintReadOnly)
		bool isJumping = false;

	void Jump();
	void StopJumping();
	//==============================================//

	//===================Aiming=====================//
	UPROPERTY(BlueprintReadOnly)
		bool isAiming = false;

	virtual void AimDownSight();
	virtual void StopAiming();

	FRotator DeltaRotator;

	UPROPERTY(BlueprintReadOnly)
		float NewYaw;
	UPROPERTY(BlueprintReadOnly)
		float NewPitch;

	/** Custom RInterpRot function using SLERP */
	UFUNCTION(BlueprintCallable, Category = "Interpolation")
		void CustomRInterpTo(FRotator current, float Interp_Speed);
	//==============================================//

	//===================Fire=======================//

	// Spawned rifle
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapons, meta = (AllowPrivateAccess = "true"))
	class ABaseWeapon* SpawnedRifle;

	// Current Weapon equipped for the character
	class ABaseWeapon* CurrentWeapon;
	/* Get the current rifle the character is holding*/
	class ABaseWeapon* GetCurrentWeapon();
	/* Set the current weapon */
	void SetCurrentWeapon(ABaseWeapon* Weapon);

	UPROPERTY(BlueprintReadOnly)
		bool isFiring = false;

	UPROPERTY(BlueprintReadWrite)
		bool CanFire = true;

	virtual void Fire();
	void StopFiring();

	UPROPERTY(BlueprintReadWrite)
		bool isReloading = false;
	
	/** Reload the weapon */
	virtual void BeginReload();
	/** Reset isReloading so we can reload again */
	virtual void EndReload();
	//==============================================//

	//=============Sprint Functionality=============//

protected:
	// Default speed 
	UPROPERTY(EditAnywhere, Category = "MovementSpeeds")
		float DefaultSpeed = 400.0f;
	// Sprint speed
	UPROPERTY(EditAnywhere, Category = "MovementSpeeds")
		float SprintSpeed = 600.0f;
	// Walk speed
	UPROPERTY(EditAnywhere, Category = "MovementSpeeds")
		float WalkSpeed = 200.0f;

public:
	/** Called for sprint input */
	void Sprint();
	/** Called to reset the walking speed to default */
	void StopSprinting();
	//==============================================//

	//===============Health System==================//

	// The health of the character
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Health")
		float HealthWeight = 100;

	UPROPERTY(BlueprintReadOnly)
		bool isDead = false;

	UPROPERTY(BlueprintReadWrite)
		bool bReceivedHit = false;

	/* Calculate the health of the character */
	void CalcHealth(float DamageDealt);
	FTimerHandle TimerHandle;
	// Destroy the character after a set interval
	void DelayedDestroy();

	/* Called when hit by a projectile */
	void DealDamage_Implementation(float Damage);
	//==============================================//
};
