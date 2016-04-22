// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/BaseWeapon.h"
#include "Pistol.generated.h"

/**
 * 
 */
UCLASS()
class FINAL_PROJECT_API APistol : public ABaseWeapon
{
	GENERATED_BODY()

public:
	APistol();
	
	virtual double CalculateDesirability(double Dist) override;
	
};
