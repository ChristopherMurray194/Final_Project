// Fill out your copyright notice in the Description page of Project Settings.

#include "Final_Project.h"
#include "Variable.h"
#include "FzSet.h"
#include "FzAND.h"
#include "FuzzyModule.h"
#include "Pistol.h"

APistol::APistol()
{
	// Max ammo in one clip
	ClipSize = 18;
	// Ammo per clip/magazine
	AmmoCount = ClipSize;
	// Rounds per second
	SetRPS(5.0f);
	// Range of rifle
	SetRange(1000.0f);
	// Damage values
	SetPlayerDamage(0.5f);
	SetEnemyDamage(10.0f);
}

double APistol::CalculateDesirability(double Dist, double Ammo)
{
	class FuzzyModule fm;

	// Fuzzy Linguistic Variables for the weapons
	class Variable& Distance = fm.CreateFLV("Distance");
	class FzSet TargetClose = Distance.AddLeftShoulderSet("TargetClose", 0, 250, 500);
	class FzSet TargetMedium = Distance.AddTriangleSet("TargetMedium", 250, 750, 1750);
	class FzSet TargetFar = Distance.AddRightShoulderSet("TargetFar", 750, 1750, 3000);

	class Variable& AmmoStatus = fm.CreateFLV("Ammo");
	class FzSet AmmoLow = AmmoStatus.AddLeftShoulderSet("AmmoLow", 0, 2, 5);
	class FzSet AmmoMedium = AmmoStatus.AddTriangleSet("AmmoMedium", 2, 5, 10);
	class FzSet AmmoHigh = AmmoStatus.AddRightShoulderSet("AmmoHigh", 10, 14, 18);

	class Variable& Desirability = fm.CreateFLV("Desirability");
	class FzSet Undesirable = Desirability.AddLeftShoulderSet("Undesirable", 0, 25, 50);
	class FzSet Desirable = Desirability.AddTriangleSet("Desirable", 25, 50, 75);
	class FzSet VeryDesirable = Desirability.AddRightShoulderSet("Very Desirable", 50, 75, 100);

	fm.AddRule(TargetClose, VeryDesirable);
	fm.AddRule(TargetMedium, Desirable);
	fm.AddRule(TargetFar, Undesirable);
	fm.AddRule(AmmoLow, Undesirable);
	fm.AddRule(AmmoMedium, Desirable);
	fm.AddRule(AmmoHigh, VeryDesirable);

	// Fuzzify the inputs
	fm.Fuzzify("Distance", Dist);
	fm.Fuzzify("AmmoStatus", Ammo);

	// Return the defuzzified crisp desirability value
	return fm.Defuzzify("Desirability", FuzzyModule::centroid);

}
