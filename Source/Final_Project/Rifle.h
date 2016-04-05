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

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called when the fire key is pressed
	bool PullTrigger();
	// Called when the fire key is released
	bool ReleaseTrigger();

	//=================Ammo========================//
	/** Calculate the remaining ammo */
	int CalculateAmmo();

	// AmmoCount mutator
	void SetAmmoCount(int Delta);

	// ClipSize Accessor
	uint8 GetClipSize();
	//=============================================//
	
private:
	// Timer handle
	FTimerHandle TimerHandle;

	void Fire();
	// Rounds per second
	float RPS = 10.0f;

	int ClipSize = 30;
	// Ammo per clip/magazine
	uint8 AmmoCount = ClipSize;
};
