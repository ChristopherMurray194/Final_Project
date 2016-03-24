// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "Final_ProjectCharacter.generated.h"

UCLASS(config=Game)
class AFinal_ProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AFinal_ProjectCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	//==================Crouching===================//
	
	// Crouch button is pressed
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Crouching")
		bool crouch_button_down = false;

	void Crouch();
	void UnCrouch();
	//==============================================//

	//===================Jumping====================//
	
	// Jump button is pressed
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Jumping")
		bool jump_button_down = false;
	
	void Jump();
	void StopJumping();
	//==============================================//

	//=============Sprint Functionality=============//
	//UPROPERTY(EditAnywhere, Category = "Sprint")
		//UAnimSequence *SprintAnim;

	// Default speed 
	UPROPERTY(EditAnywhere, Category = "MovementSpeeds")
		float defaultSpeed = 400.0f;
	// Sprint speed
	UPROPERTY(EditAnywhere, Category = "MovementSpeeds")
		float SprintSpeed = 800.0f;

	/** Called for sprint input */
	void Sprint();
	/** Called to reset the walking speed to default */
	void StopSprinting();
	//==============================================//

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

