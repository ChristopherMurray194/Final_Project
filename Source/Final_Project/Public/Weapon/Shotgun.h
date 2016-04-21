// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/BaseWeapon.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class FINAL_PROJECT_API AShotgun : public ABaseWeapon
{
	GENERATED_BODY()

public:
	AShotgun();
	
	virtual double CalculateDesirability(double Dist, double Ammo) override;
	
	
};
