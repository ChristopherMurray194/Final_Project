// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "BaseCharacter.h"
#include "Final_ProjectCharacter.generated.h"

UCLASS(config=Game)
class AFinal_ProjectCharacter : public ABaseCharacter
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

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	// Location line trace from weapon hits, to be referenced for the reticle location.
	UPROPERTY(BlueprintReadOnly)
		FVector2D ReticleLocation;
	
	FVector2D ReticleDefaultLocation;
	UPROPERTY(EditAnywhere, Category = Crosshair)
		// X coordinate for the crosshair
		float ReticleX = -70.0f;
	UPROPERTY(EditAnywhere, Category = Crosshair)
		// Y coordinate for the crosshair
		float ReticleY = -60.0f;

	/* The HUD asset */
	UPROPERTY(EditAnywhere, Category = HUD)
		TSubclassOf<class UUserWidget> HUD;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

private:
	FLinearColor PlayerColor = FLinearColor(0.0f, (128.0f / 255.0f), (128.0f / 255.0f), 1.0f);

	/* When Keyboard asterix key pressed twice, character does not use controller yaw */
	void ToggleControllerYaw();
	bool bUnlockYaw = false;	// Toggle between controller yaw

	float DefaultCameraDist = 200.0f; // Default distance of the camera from the player

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

	//============Crouching==============//
	bool bStopCrouching = false;
	
	//**** If the editor crashes suddenly the overriding of these virtual functions may be the cause. ****//
	void Crouch();
	void UnCrouch();
	//===================================//

	//============Prone==================//
	bool bCanStand = false;

	virtual void GoProne() override;
	//===================================//

	//=============Aiming================//
	virtual void AimDownSight() override;
	virtual void StopAiming() override;
	//===================================//

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

