// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Rifle.generated.h"

UCLASS()
class FINAL_PROJECT_API ARifle : public AActor
{
	GENERATED_BODY()

public:
	class USkeletalMeshComponent* GunMesh;
	class UArrowComponent* ArrowComp;

	// Sets default values for this actor's properties
	ARifle();

	void TraceLine(FHitResult* Hit);

	// Called when the fire key is pressed
	bool PullTrigger();
	// Called when the fire key is released
	bool ReleaseTrigger();

	//=================Ammo========================//

	/** Calculate the remaining ammo */
	int32 CalculateAmmo();
	// AmmoCount mutator
	void SetAmmoCount(int Delta);

	// ClipSize Accessor
	int32 GetClipSize();

	// Max ammo in one clip
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		int32 ClipSize = 30;

	// Ammo per clip/magazine
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		int32 AmmoCount = ClipSize;
	//=============================================//
	
private:
	// Timer handle
	FTimerHandle TimerHandle;

	void Fire();
	// Rounds per second
	float RPS = 10.0f;

};
