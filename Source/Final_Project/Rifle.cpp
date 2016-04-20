// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "Rifle.h"

// Sets default values
ARifle::ARifle()
{
	// Max ammo in one clip
	ClipSize = 30;
	// Ammo per clip/magazine
	AmmoCount = ClipSize;
	// Rounds per second
	SetRPS(10.0f);
	// Range of rifle
	SetRange(1000.0f);
	// Damage values
	SetPlayerDamage(0.5f);
	SetEnemyDamage(20.0f);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GunAsset(TEXT("/Game/FPWeapon/Mesh/SK_FPGun.SK_FPGun"));
	if (GunAsset.Succeeded())
	{
		static ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Game/FPWeapon/Materials/M_FPGun"));
		if (MaterialAsset.Succeeded())
		{
			SetupWeaponMesh(GunAsset.Object, MaterialAsset.Object, FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, -90.0f, 0.0f));
		}
		SetupArrowComp(FVector(0.0f, 50.0f, 11.0f), FRotator(0.0f, 90.0f, 0.0f));
	}
}