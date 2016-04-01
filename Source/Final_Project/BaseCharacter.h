// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class FINAL_PROJECT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

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

	void AimDownSight();
	void StopAiming();
	//==============================================//

	//===================Fire=======================//

	// Spawned rifle
	class ARifle* SpawnedRifle;

	UPROPERTY(BlueprintReadOnly)
		bool isFiring = false;

	void Fire();
	void StopFiring();

	UPROPERTY(BlueprintReadOnly)
		bool isReloading = false;
	
	/** Reload the weapon */
	virtual void BeginReload();
	/** Reset isReloading so we can reload again */
	virtual void EndReload();
	//==============================================//

	//=============Sprint Functionality=============//

	// Default speed 
	UPROPERTY(EditAnywhere, Category = "MovementSpeeds")
		float defaultSpeed = 400.0f;
	// Sprint speed
	UPROPERTY(EditAnywhere, Category = "MovementSpeeds")
		float SprintSpeed = 600.0f;

	/** Called for sprint input */
	void Sprint();
	/** Called to reset the walking speed to default */
	void StopSprinting();
	//==============================================//
};
