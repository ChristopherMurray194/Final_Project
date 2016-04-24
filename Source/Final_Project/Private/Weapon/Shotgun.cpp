// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "Variable.h"
#include "FzSet.h"
#include "FzAND.h"
#include "FuzzyModule.h"
#include "Shotgun.h"

AShotgun::AShotgun()
{
	// Max ammo in one clip
	ClipSize = 8;
	// Ammo per clip/magazine
	AmmoCount = ClipSize;
	// Rounds per second
	SetRPS(1.0f);
	// Range of rifle
	SetRange(750.0f);
	// Damage values
	SetPlayerDamage(20.0f);
	SetEnemyDamage(50.0f);

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

void AShotgun::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	GunMesh->CreateAndSetMaterialInstanceDynamic(0)->SetVectorParameterValue(TEXT("BodyColor"), FLinearColor(0.f, (139.f / 255.f), 0.f));
}

double AShotgun::CalculateDesirability(double Dist)
{
	class FuzzyModule fm;

	// Fuzzy Linguistic Variables for the weapons
	class Variable& Distance = fm.CreateFLV("Distance");
	class FzSet TargetClose = Distance.AddLeftShoulderSet("TargetClose", 0, 250, 500);
	class FzSet TargetMedium = Distance.AddTriangleSet("TargetMedium", 250, 750, 1750);
	class FzSet TargetFar = Distance.AddRightShoulderSet("TargetFar", 750, 1750, 3000);

	class Variable& AmmoStatus = fm.CreateFLV("Ammo");
	class FzSet AmmoLow = AmmoStatus.AddLeftShoulderSet("AmmoLow", 0, 2, 4);
	class FzSet AmmoMedium = AmmoStatus.AddTriangleSet("AmmoMedium", 2, 4, 6);
	class FzSet AmmoHigh = AmmoStatus.AddRightShoulderSet("AmmoHigh", 4, 6, 8);

	class Variable& Desirability = fm.CreateFLV("Desirability");
	class FzSet Undesirable = Desirability.AddLeftShoulderSet("Undesirable", 0, 25, 50);
	class FzSet Desirable = Desirability.AddTriangleSet("Desirable", 25, 50, 75);
	class FzSet VeryDesirable = Desirability.AddRightShoulderSet("Very Desirable", 50, 75, 100);

	fm.AddRule(TargetClose, VeryDesirable);
	fm.AddRule(TargetMedium, Undesirable);
	fm.AddRule(TargetFar, Undesirable);
	fm.AddRule(AmmoLow, Undesirable);
	fm.AddRule(AmmoMedium, Desirable);
	fm.AddRule(AmmoHigh, VeryDesirable);

	// Fuzzify the inputs
	fm.Fuzzify("Distance", Dist);
	fm.Fuzzify("Ammo", CalculateAmmo());

	// Return the defuzzified crisp desirability value
	return fm.Defuzzify("Desirability", FuzzyModule::max_av);
}