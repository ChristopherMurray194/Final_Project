// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Weapon/BaseWeapon.h"
#include "Rifle.generated.h"

UCLASS()
class FINAL_PROJECT_API ARifle : public ABaseWeapon
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	ARifle();

	void PostInitializeComponents() override;

	virtual double CalculateDesirability(double Dist) override;
};
