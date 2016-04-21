// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

UCLASS()
class FINAL_PROJECT_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

	class USkeletalMeshComponent* GunMesh;
	class UArrowComponent* ArrowComp;

	/*
	* Setup the skeletal mesh of the weapon 
	* @param Mesh - Skeletal mesh of the weapon
	* @param Mat - Material of the weapon (assigned to index 0)
	* @param relLocation - Sets the location of the arrow component relative to its parent
	* @param relRotation - Sets the rotation of the arrow component relative to its parent
	*/
	void SetupWeaponMesh(USkeletalMesh* Mesh, UMaterial* Mat, FVector relLocation, FRotator relRocation);

	/*
	* Setup the Arrow component for the weapon
	* @param relLocation - Sets the location of the arrow component relative to its parent
	* @param relRotation - Sets the rotation of the arrow component relative to its parent
	*/
	void SetupArrowComp(FVector relLocation, FRotator relRotation);

	/*
	Create a line trace from the gun.
	@param Hit - The hit result of the line trace.
	@param DrawTrace - Whether the line trace should be visible or not.
	*/
	void TraceLine(FHitResult* Hit, bool DrawTrace);

	// Called when the fire key is pressed
	bool PullTrigger();
	// Called when the fire key is released
	bool ReleaseTrigger();

	//=================Ammo========================//

	/* Calculate the remaining ammo 
	* AmmoCount Accessor
	*/
	int32 CalculateAmmo();
	// AmmoCount mutator
	void SetAmmoCount(int32 Val);
	// ClipSize mutator
	void SetClipSize(int32 Val);

	// ClipSize Accessor
	int32 GetClipSize();

	// Max ammo in one clip
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		int32 ClipSize = 30;

	// Ammo per clip/magazine
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		int32 AmmoCount = ClipSize;
	//=============================================//


protected:
	// Timer handle
	FTimerHandle TimerHandle;

	void Fire();
	// Rounds per second
	float RPS = 10.0f;
	void SetRPS(float Val);
	float GetRPS();

	// Range of the weapon
	float Range = 0.0f;
	void SetRange(float Val);
	float GetRange();
	//===================Damage======================//

	// Amount of damage applied to the player by the weapon
	float PlayerDamage = 0.5f;
	// Amount of damage applied to the enemy by the weapon
	float EnemyDamage = 20.0f;

	void SetPlayerDamage(float Val);
	float GetPlayerDamage();
	void SetEnemyDamage(float Val);
	float GetEnemyDamage();
	//==============================================//
	
};
