// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IDamageable.generated.h"

/**
 * Interface that can be used to deal damage to an actor
 */
UINTERFACE()
class FINAL_PROJECT_API UIDamageable : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IIDamageable
{
	GENERATED_IINTERFACE_BODY()

	float DealDamage(float Damage);
};
